using System;
using System.Collections.Generic;
using System.IO;


namespace SuburbanDigitalEnginePreprocessor {
    class FileProcessor {
        public string FilePath { get; private set; }
        public string FileName { get; private set; }
        public string OutputFilePath { get; private set; }

        private enum EType {
            Struct, Class
        }
        private EType CompilationUnitType;
        private enum ECompilingState {
            Starting,
            Completed,
            Declaring,
            AddingMembers,
            AddingParentClasses
        }
        private ECompilingState CurrentState;

        public FileProcessor(string FilePath, string DestinationPath) {
            this.FilePath = FilePath;
            FileName = Path.GetFileName(FilePath);
            OutputFilePath = Path.Combine(DestinationPath, FileName.Split('.')[0] + ".reflected.h");
            CurrentState = ECompilingState.Starting;
        }

        public void ProcessFile() {
            List<string> lines = ParseFileIntoLines();
            List<Tuple<EToken, string>> tokens = TokenizeFile(lines);

            if (tokens.Count > 0) {
                Console.WriteLine($"Encountered reflection macros in file: {FileName}.");
                List<string> fileContents = LexTokensIntoReflectionFile(tokens);
                WriteOutputFile(fileContents);
            }
        }
        public List<string> LexTokensIntoReflectionFile(List<Tuple<EToken, string>> TokenizedLines) {
            List<string> fileContents = new List<string>();
            fileContents.Add("#pragma once");
            fileContents.Add("#include \"Core/Reflection/Reflection.h\"");
            fileContents.Add("#include \"" + FilePath + "\"");
            fileContents.Add("");

            for (int i = 0; i < TokenizedLines.Count; i++) {
                while (!EnsureValidState(ref fileContents, TokenizedLines[i])) {

                }
                EToken token = TokenizedLines[i].Item1;
                switch (token) {
                    case EToken.StructDeclaration:
                        ProcessStructDeclaration(ref fileContents, TokenizedLines[i]);
                        CompilationUnitType = EType.Struct;
                        CurrentState = ECompilingState.AddingParentClasses;
                        break;

                    case EToken.ClassDeclaration:
                        ProcessClassDeclaration(ref fileContents, TokenizedLines[i]);
                        CompilationUnitType = EType.Class;
                        CurrentState = ECompilingState.AddingParentClasses;
                        break;

                    case EToken.PropertyDeclaration:
                        ProcessPropertyDeclaration(ref fileContents, TokenizedLines[i]);
                        CurrentState = ECompilingState.AddingMembers;
                        break;

                    case EToken.ParentClassDeclaration:
                        ProcessParentClassDeclaration(ref fileContents, TokenizedLines[i]);
                        CurrentState = ECompilingState.AddingParentClasses;
                        break;
                }
            }

            // Cleanup
            if(CurrentState == ECompilingState.AddingParentClasses) {
                if (CompilationUnitType == EType.Class) {
                    fileContents.Add("REFLECT_CLASS_PARENT_END()");
                    fileContents.Add("REFLECT_CLASS_MEMBERS_BEGIN()");
                } else {
                    fileContents.Add("REFLECT_STRUCT_PARENT_END()");
                    fileContents.Add("REFLECT_STRUCT_MEMBERS_BEGIN()");
                }
                CurrentState = ECompilingState.AddingMembers;
            }
            if(CurrentState == ECompilingState.AddingMembers) {
                if (CompilationUnitType == EType.Class) {
                    fileContents.Add("REFLECT_CLASS_MEMBERS_END()");
                    fileContents.Add("REFLECT_CLASS_END()");
                } else {
                    fileContents.Add("REFLECT_STRUCT_MEMBERS_END()");
                    fileContents.Add("REFLECT_STRUCT_END()");
                }
            }

            return fileContents;
        }
        void ProcessStructDeclaration(ref List<string> FileContents, Tuple<EToken, string> TokenizedLine) {
            string structName = TokenizedLine.Item2.Split(' ')[1];
            string outputLine = "REFLECT_STRUCT_BEGIN(" + structName + ")";
            FileContents.Add("//Reflection for struct " + structName);
            FileContents.Add(outputLine);
            FileContents.Add("REFLECT_STRUCT_BEGIN_PARENTS()");
        }
        void ProcessClassDeclaration(ref List<string> FileContents, Tuple<EToken, string> TokenizedLine) {
            string className = TokenizedLine.Item2;
            string outputLine = "REFLECT_CLASS_BEGIN(" + className + ")";
            FileContents.Add("//Reflection for struct " + className);
            FileContents.Add(outputLine);
            FileContents.Add("REFLECT_CLASS_BEGIN_PARENTS()");
        }
        void ProcessPropertyDeclaration(ref List<string> FileContents, Tuple<EToken, string> TokenizedLine) {
            string outputLine = "REFLECT_STRUCT_MEMBER(";
            outputLine += TokenizedLine.Item2.Split(' ')[1].Split('[')[0];
            outputLine += ")";
            FileContents.Add(outputLine);
        }
        void ProcessParentClassDeclaration(ref List<string> FileContents, Tuple<EToken, string> TokenizedLine) {
            string outputLine = "REFLECT_CLASS_PARENT(";
            outputLine += TokenizedLine.Item2;
            outputLine += ")";
            FileContents.Add(outputLine);
        }
        bool EnsureValidState(ref List<string> FileContents, Tuple<EToken, string> TokenizedLine) {
            if (CurrentState == ECompilingState.AddingParentClasses && TokenizedLine.Item1 != EToken.ParentClassDeclaration) {
                if (CompilationUnitType == EType.Class) {
                    FileContents.Add("REFLECT_CLASS_PARENT_END()");
                    FileContents.Add("REFLECT_CLASS_MEMBERS_BEGIN()");
                } else {
                    FileContents.Add("REFLECT_STRUCT_PARENT_END()");
                    FileContents.Add("REFLECT_STRUCT_MEMBERS_BEGIN()");
                }
                CurrentState = ECompilingState.AddingMembers;
                return false;
            }
            if (CurrentState == ECompilingState.AddingMembers && TokenizedLine.Item1 != EToken.PropertyDeclaration) {
                if (CompilationUnitType == EType.Class) {
                    FileContents.Add("REFLECT_CLASS_MEMBERS_END()");
                    FileContents.Add("REFLECT_CLASS_END()");
                } else {
                    FileContents.Add("REFLECT_STRUCT_MEMBERS_END()");
                    FileContents.Add("REFLECT_STRUCT_END()");
                }
                CurrentState = ECompilingState.Completed;
                return false;
            }

            return true;
        }
        public List<Tuple<EToken, string>> TokenizeFile(List<string> ParsedLines) {
            List<Tuple<EToken, string>> tokens = new List<Tuple<EToken, string>>();

            for (int i = 0; i < ParsedLines.Count; i++) {
                string line = ParsedLines[i];
                if (Macros.IsValidMacro(line)) {
                    EToken token = Macros.GetMacroFromMacroString(line);
                    string followingLine = ParsedLines[i + 1];
                    followingLine = followingLine.Replace("{", "");
                    followingLine = followingLine.Replace("}", "");
                    followingLine = followingLine.Replace(";", "");

                    if (token == EToken.ClassDeclaration) {
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
        public List<string> ParseFileIntoLines() {
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
        public void WriteOutputFile(List<string> ReflectedFileContents) {
            File.WriteAllLines(OutputFilePath, ReflectedFileContents);
        }
    }
}
