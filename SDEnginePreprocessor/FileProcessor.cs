using System;
using System.Collections.Generic;
using System.IO;


namespace SuburbanDigitalEnginePreprocessor {
    class FileProcessor {
        public string FilePath { get; private set; }
        public string FileName { get; private set; }
        public string OutputFilePath { get; private set; }
        public Dictionary<string, CPPObject> Objects { get; private set; }

        public FileProcessor(string FilePath, string DestinationPath) {
            this.FilePath = FilePath;
            FileName = Path.GetFileName(FilePath);
            OutputFilePath = Path.Combine(DestinationPath, FileName.Split('.')[0] + ".reflected.h");
            Objects = new Dictionary<string, CPPObject>();
        }
        public bool ProcessFile() {
            List<string> lines = ParseFileIntoLines();
            List<Tuple<EToken, string>> tokens = TokenizeFile(lines);
            CreateObjects(tokens);
            if(tokens.Count > 0) {
                return true;
            }
            return false;
        }
        public void GenerateReflectedFile() {
            //ResolveParentClasses();
            WriteReflectedHeaderOut();
        }
        private void ResolveParentClasses() {
            foreach (CPPObject encapsulator in Objects.Values) {
                List<string> propertiesToAdd = new List<string>();
                foreach (string parentClass in encapsulator.ParentClasses) {
                    ResolveParentsWorker(parentClass, propertiesToAdd);
                }
                foreach(string prop in propertiesToAdd) {
                    encapsulator.AddProperty(prop);
                }
            }
        }
        private void ResolveParentsWorker(string ParentClass, List<string> Properties) {
            if(!Globals.ReflectedObjects.ContainsKey(ParentClass)) {
                return;
            }
            foreach(string property in Globals.ReflectedObjects[ParentClass].Properties) {
                Properties.Add(property);
            }
            foreach(string parentClass in Globals.ReflectedObjects[ParentClass].ParentClasses) {
                ResolveParentsWorker(parentClass, Properties);
            }
        }
        private void CreateObjects(List<Tuple<EToken, string>> Tokens) {
            CPPObject current = null;
            foreach (Tuple<EToken, string> token in Tokens) {
                switch (token.Item1) {
                    case EToken.ClassDeclaration:
                        if(current != null) {
                            Globals.ReflectedObjects.Add(current.Name, current.GetCopy());
                            Objects.Add(current.Name, current);
                        }
                        current = new CPPObject(EReflectionTarget.CLASS, token.Item2);
                        break;
                    case EToken.StructDeclaration:
                        if (current != null) {
                            Globals.ReflectedObjects.Add(current.Name, current.GetCopy());
                            Objects.Add(current.Name, current);
                        }
                        current = new CPPObject(EReflectionTarget.STRUCT, token.Item2);
                        break;
                    case EToken.ParentClassDeclaration:
                        current.AddParentClass(token.Item2);
                        break;
                    case EToken.PropertyDeclaration:
                        string[] split = token.Item2.Split(' ');
                        if(split[0].Trim() == "const") {
                            current.AddProperty(split[2]); // const int32 variableName = 10;
                        } else {
                            current.AddProperty(split[1]); // int32 variableName = 10;
                        }
                        break;
                    default: break;
                }
            }
            if (current != null) {
                Globals.ReflectedObjects.Add(current.Name, current.GetCopy());
                Objects.Add(current.Name, current);
            }
        }
        private List<Tuple<EToken, string>> TokenizeFile(List<string> ParsedLines) {
            List<Tuple<EToken, string>> tokens = new List<Tuple<EToken, string>>();

            for (int i = 0; i < ParsedLines.Count; i++) {
                string line = ParsedLines[i];
                if (Macros.IsValidMacro(line)) {
                    EToken token = Macros.GetMacroFromMacroString(line);
                    string followingLine = ParsedLines[i + 1];
                    followingLine = followingLine.Replace("{", "");
                    followingLine = followingLine.Replace("}", "");
                    followingLine = followingLine.Replace(";", "");

                    if (token == EToken.ClassDeclaration || token == EToken.StructDeclaration) {
                        string[] declarationAndParents = followingLine.Split(':');
                        tokens.Add(new Tuple<EToken, string>(token, declarationAndParents[0].Split(' ')[1]));

                        if (declarationAndParents.Length > 1) {
                            for (int j = 1; j < declarationAndParents.Length; j += 2) {
                                string candidate = declarationAndParents[j].Trim();
                                tokens.Add(new Tuple<EToken, string>(EToken.ParentClassDeclaration, candidate.Split(' ')[1]));
                            }
                        }
                    } else {
                        tokens.Add(new Tuple<EToken, string>(token, followingLine));
                    }
                }
            }
            return tokens;
        }
        private List<string> ParseFileIntoLines() {
            StreamReader reader = File.OpenText(FilePath);
            string fileLine;
            List<string> lines = new List<string>();
            while ((fileLine = reader.ReadLine()) != null) {
                if (!string.IsNullOrWhiteSpace(fileLine) && !fileLine.StartsWith("/")) {
                    lines.Add(fileLine.Trim());
                }
            }
            return lines;
        }
        private void WriteReflectedHeaderOut() {
            List<string> fileContents = new List<string>();
            fileContents.Add("#pragma once");
            fileContents.Add("#include \"Core/Reflection/Reflection.h\"");
            fileContents.Add("#include \"" + FilePath + "\"");
            fileContents.Add("");

            foreach (CPPObject enc in Objects.Values) {
                fileContents.Add("");
                if (enc.Type == EReflectionTarget.CLASS) {
                    fileContents.Add($"//Reflection for class {enc.Name}");
                    fileContents.Add($"REFLECT_CLASS_BEGIN({enc.Name})");
                    fileContents.Add($"REFLECT_CLASS_BEGIN_PARENTS()");
                    foreach(string parent in enc.ParentClasses) {
                        fileContents.Add($"REFLECT_CLASS_PARENT({parent})");
                    }
                    fileContents.Add($"REFLECT_CLASS_PARENT_END()");
                    fileContents.Add($"REFLECT_CLASS_MEMBERS_BEGIN()");
                    foreach (string property in enc.Properties) {
                        fileContents.Add($"REFLECT_CLASS_MEMBER({property})");
                    }
                    fileContents.Add($"REFLECT_CLASS_MEMBERS_END()");
                    fileContents.Add($"REFLECT_CLASS_END()");
                } else if(enc.Type == EReflectionTarget.STRUCT) {
                    fileContents.Add($"//Reflection for struct {enc.Name}");
                    fileContents.Add($"REFLECT_STRUCT_BEGIN({enc.Name})");
                    fileContents.Add($"REFLECT_STRUCT_BEGIN_PARENTS()");
                    foreach (string parent in enc.ParentClasses) {
                        fileContents.Add($"REFLECT_STRUCT_PARENT({parent})");
                    }
                    fileContents.Add($"REFLECT_STRUCT_PARENT_END()");
                    fileContents.Add($"REFLECT_STRUCT_MEMBERS_BEGIN()");
                    foreach (string property in enc.Properties) {
                        fileContents.Add($"REFLECT_STRUCT_MEMBER({property})");
                    }
                    fileContents.Add($"REFLECT_STRUCT_MEMBERS_END()");
                    fileContents.Add($"REFLECT_STRUCT_END()");
                }
            }

            File.WriteAllLines(OutputFilePath, fileContents);
        }
    }
}
