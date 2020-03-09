using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace SuburbanDigitalEnginePreprocessor {
    class FileProcessor {
        public string SourceFilePath { get; private set; }
        public string FileName { get; private set; }
        public string OutputFilePath { get; private set; }
        public Dictionary<string, CPPObject> Objects { get; private set; }

        public FileProcessor(string FilePath, string DestinationPath) {
            this.SourceFilePath = FilePath;
            FileName = Path.GetFileName(FilePath);
            OutputFilePath = Path.Combine(DestinationPath, FileName.Split('.')[0] + ".reflected.h");
            Objects = new Dictionary<string, CPPObject>();
        }
        public bool ProcessFile() {
            List<string> lines = ParseFileIntoLines();
            List<Tuple<EToken, string, string>> tokens = TokenizeFile(lines);
            CreateObjects(tokens);
            if (tokens.Count > 0) {
                return true;
            }
            return false;
        }
        public void GenerateReflectedFile() {
            WriteReflectedHeaderOut();
        }
        public bool ContainsReflectableContents() {
            return Objects.Count > 0;
        }
        public bool HasSourceFileChanged() {
            // Check to see if both the reflected and source files exist. If true, check the cached last
            // modified time of the source file that was recorded in the reflected file. If the modified
            // time of the source file is greater than the cached time stored in the reflected file, we
            // assume the source file is dirty and we must re-reflect.
            if (File.Exists(OutputFilePath) && File.Exists(SourceFilePath)) {
                string[] lines = File.ReadAllLines(OutputFilePath);
                if (lines[0].StartsWith("//LastModifiedTime:")) {
                    string[] split = lines[0].Split("//LastModifiedTime:");
                    if (split.Length > 1) {
                        string lastModified = split[1].Trim();
                        long lastModifiedTicks = Convert.ToInt64(lastModified);
                        return File.GetLastWriteTime(SourceFilePath).Ticks > lastModifiedTicks;
                    }
                }
            }
            return true;
        }
        private void ResolveParentClasses() {
            foreach (CPPObject encapsulator in Objects.Values) {
                List<FProperty> propertiesToAdd = new List<FProperty>();
                foreach (string parentClass in encapsulator.ParentClasses) {
                    ResolveParentsWorker(parentClass, propertiesToAdd);
                }
                foreach (FProperty prop in propertiesToAdd) {
                    encapsulator.AddProperty(prop);
                }
            }
        }
        private void ResolveParentsWorker(string ParentClass, List<FProperty> Properties) {
            //if(!Globals.ReflectedObjects.ContainsKey(ParentClass)) {
            //    return;
            //}
            //foreach(string property in Globals.ReflectedObjects[ParentClass].Properties) {
            //    Properties.Add(property);
            //}
            //foreach(string parentClass in Globals.ReflectedObjects[ParentClass].ParentClasses) {
            //    ResolveParentsWorker(parentClass, Properties);
            //}
        }
        private void CreateObjects(List<Tuple<EToken, string, string>> Tokens) {
            CPPObject current = null;
            foreach (Tuple<EToken, string, string> token in Tokens) {
                switch (token.Item1) {
                    case EToken.ClassDeclaration:
                        if (current != null) {
                            Globals.ReflectedObjects.Add(current.Name, current.GetCopy());
                            Objects.Add(current.Name, current);
                        }
                        current = new CPPObject(EReflectionTarget.CLASS, token.Item3);
                        break;
                    case EToken.StructDeclaration:
                        if (current != null) {
                            Globals.ReflectedObjects.Add(current.Name, current.GetCopy());
                            Objects.Add(current.Name, current);
                        }
                        current = new CPPObject(EReflectionTarget.STRUCT, token.Item3);
                        break;
                    case EToken.ParentClassDeclaration:
                        current.AddParentClass(token.Item3);
                        break;
                    case EToken.PropertyDeclaration:
                        string[] split = token.Item3.Split(' ', StringSplitOptions.RemoveEmptyEntries); ;
                        if (split[0].Trim() == "const") {
                            current.AddProperty(new FProperty(split[2], token.Item2)); // const int32 variableName = 10;
                        } else {
                            current.AddProperty(new FProperty(split[1], token.Item2)); // int32 variableName = 10;
                        }
                        break;
                    case EToken.FunctionDeclaration:
                        current.AddFunction(new FFunction(token.Item3, token.Item2));
                        break;
                    default: break;
                }
            }
            if (current != null) {
                Globals.ReflectedObjects.Add(current.Name, current.GetCopy());
                Objects.Add(current.Name, current);
            }
        }
        private List<Tuple<EToken, string, string>> TokenizeFile(List<string> ParsedLines) {
            List<Tuple<EToken, string, string>> tokens = new List<Tuple<EToken, string, string>>();

            for (int i = 0; i < ParsedLines.Count; i++) {
                string line = ParsedLines[i];
                if (Macros.IsValidMacro(line)) {
                    // Determine what kind of token this is and then perform a lookahead.
                    EToken token = Macros.GetMacroFromMacroString(line);
                    string followingLine = RemoveUnparseableTokens(ParsedLines[i + 1]);

                    // If this is a declaration token, we must perform some additional parsing.
                    if (token == EToken.ClassDeclaration || token == EToken.StructDeclaration) {
                        //Split the line along the color (ie. class Test : 'public Parent1, public Parent2, Parent3, private Parent4').
                        string[] declarationAndParents = followingLine.Split(':', StringSplitOptions.RemoveEmptyEntries);

                        // Capture the declaration name (ie. 'class EntityName' turns into 'EntityName').
                        string declaringName = declarationAndParents[0].Split(' ', StringSplitOptions.RemoveEmptyEntries)[1].Trim();
                        tokens.Add(new Tuple<EToken, string, string>(token, line, declaringName));

                        // Capture any parent declarations as well. Split the parent portion of the declaration by comma.
                        if (declarationAndParents.Length > 1) {
                            // Split the parent declarations into tokens in the form of either 'public Parent' or just 'Parent'.
                            string[] parents = declarationAndParents[1].Trim().Split(',', StringSplitOptions.RemoveEmptyEntries);

                            // Iterate through all parents and remove any leading or trailing spaces. Then see if there is
                            // a space inside the declaration. If there is, this is a parent declaration in the form of
                            // 'public Parent', otherwise this is just a 'Parent'.
                            for (int j = 0; j < parents.Length; j++) {
                                // Check to see if this is a two keyword or single keyword declaration.
                                string parentPair = parents[j].Trim();
                                string parent = "";
                                if (parentPair.Contains(' ')) {
                                    parent = parentPair.Split(' ', StringSplitOptions.RemoveEmptyEntries)[1];
                                } else {
                                    parent = parentPair;
                                }
                                tokens.Add(new Tuple<EToken, string, string>(EToken.ParentClassDeclaration, line, parent));
                            }
                        }
                    } else {
                        tokens.Add(new Tuple<EToken, string, string>(token, line, followingLine));
                    }
                }
            }
            return tokens;
        }
        private List<string> ParseFileIntoLines() {
            StreamReader reader = File.OpenText(SourceFilePath);
            string fileLine;
            List<string> lines = new List<string>();
            while ((fileLine = reader.ReadLine()) != null) {
                if (!string.IsNullOrWhiteSpace(fileLine) && !fileLine.StartsWith("//")) {
                    lines.Add(fileLine.Trim());
                }
            }
            return lines;
        }
        private void WriteReflectedHeaderOut() {
            DateTime lastWriteTime = File.GetLastWriteTimeUtc(OutputFilePath);
            List<string> fileContents = new List<string>();
            fileContents.Add("//LastModifiedTime:" + lastWriteTime.Ticks.ToString());
            fileContents.Add("#pragma once");
            fileContents.Add("#include \"Core/Reflection/Reflection.h\"");
            fileContents.Add("#include \"" + SourceFilePath + "\"");
            fileContents.Add("");

            foreach (CPPObject enc in Objects.Values) {
                fileContents.Add("");
                if (enc.Type == EReflectionTarget.CLASS) {
                    fileContents.Add($"//Reflection for class {enc.Name}");
                    fileContents.Add($"REFLECT_CLASS_BEGIN({enc.Name})");
                    // Parent Classes
                    fileContents.Add($"REFLECT_CLASS_BEGIN_PARENTS()");
                    foreach (string parent in enc.ParentClasses) {
                        if (Globals.ReflectedObjects.ContainsKey(parent)) {
                            fileContents.Add($"REFLECT_CLASS_PARENT({parent})");
                        }
                    }
                    fileContents.Add($"REFLECT_CLASS_PARENT_END()");

                    // Members
                    foreach (FProperty property in enc.Properties.Values.OrderBy(i => i.GetCategory())) {
                        fileContents.Add($"REFLECT_CLASS_MEMBER({property.GetMacroInternals()})");
                    }

                    // Functions
                    foreach (FFunction function in enc.Functions.OrderBy(i => i.GetCategory())) {
                        fileContents.Add($"REFLECT_CLASS_FUNCTION({function.GetMacroInternals()})");
                    }

                    fileContents.Add($"REFLECT_CLASS_END()");
                } else if (enc.Type == EReflectionTarget.STRUCT) {
                    fileContents.Add($"//Reflection for struct {enc.Name}");
                    fileContents.Add($"REFLECT_STRUCT_BEGIN({enc.Name})");
                    fileContents.Add($"REFLECT_STRUCT_BEGIN_PARENTS()");
                    foreach (string parent in enc.ParentClasses) {
                        if (Globals.ReflectedObjects.ContainsKey(parent)) {
                            fileContents.Add($"REFLECT_STRUCT_PARENT({parent})");
                        }
                    }
                    fileContents.Add($"REFLECT_STRUCT_PARENT_END()");
                    fileContents.Add($"REFLECT_STRUCT_MEMBERS_BEGIN()");
                    foreach (FProperty property in enc.Properties.Values.OrderBy(i => i.GetCategory())) {
                        fileContents.Add($"REFLECT_STRUCT_MEMBER({property.GetMacroInternals()})");
                    }
                    fileContents.Add($"REFLECT_STRUCT_MEMBERS_END()");
                    fileContents.Add($"REFLECT_STRUCT_END()");
                }
            }

            File.WriteAllLines(OutputFilePath, fileContents);
        }
        private string RemoveUnparseableTokens(string Line) {
            Line = Line.Replace("{", "");
            Line = Line.Replace("}", "");
            Line = Line.Replace(";", "");
            return Line;
        }
    }
}
