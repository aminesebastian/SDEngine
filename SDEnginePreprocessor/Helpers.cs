using System;
using System.Collections.Generic;
using System.Text;

namespace SuburbanDigitalEnginePreprocessor {
    enum EToken {
        StructDeclaration, ClassDeclaration, ParentClassDeclaration, PropertyDeclaration, INVALID
    }
    enum EReflectionTarget {
        CLASS, STRUCT
    }
    class Globals {
        public static Dictionary<string, CPPObject> ReflectedObjects = new Dictionary<string, CPPObject>();
    }
    class Macros {
        private static readonly HashSet<string> VALID_MACROS;

        static Macros() {
            VALID_MACROS = new HashSet<string>();
            VALID_MACROS.Add("SD_STRUCT");
            VALID_MACROS.Add("SD_CLASS");
            VALID_MACROS.Add("SD_PROPERTY");
        }
        public static bool IsValidMacro(string MacroString) {
            if(!MacroString.StartsWith("SD_")) {
                return false;
            }
            string cleanedMacroString = GetMacroNameFromMacroLine(MacroString);
            return VALID_MACROS.Contains(cleanedMacroString);
        }
        public static EToken GetMacroFromMacroString(string MacroString) {
            string cleanedMacroString = GetMacroNameFromMacroLine(MacroString);
            switch(cleanedMacroString) {
                case "SD_STRUCT": return EToken.StructDeclaration;
                case "SD_CLASS": return EToken.ClassDeclaration;
                case "SD_PROPERTY": return EToken.PropertyDeclaration;
                default: return EToken.INVALID;
            }
        }
        private static string GetMacroNameFromMacroLine(string MacroLine) {
            if (!MacroLine.StartsWith("SD_")) {
                return "";
            }
            string cleanedMacroString = MacroLine.Split(';')[0];
            int indexOfFirstParen = cleanedMacroString.IndexOf('(');
            if (indexOfFirstParen > 0) {
                return cleanedMacroString.Substring(0, indexOfFirstParen);
            }
            return "";
        }
    }
}
