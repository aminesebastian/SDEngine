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
    class Macros {
        private static readonly HashSet<string> VALID_MACROS;

        static Macros() {
            VALID_MACROS = new HashSet<string>();
            VALID_MACROS.Add("SD_STRUCT()");
            VALID_MACROS.Add("SD_CLASS()");
            VALID_MACROS.Add("SD_PROPERTY()");
        }
        public static bool IsValidMacro(string MacroString) {
            string cleanedMacroString = MacroString.Split(';')[0];
            return VALID_MACROS.Contains(cleanedMacroString);
        }
        public static EToken GetMacroFromMacroString(string MacroString) {
            string cleanedMacroString = MacroString.Split(';')[0];
            switch(cleanedMacroString) {
                case "SD_STRUCT()": return EToken.StructDeclaration;
                case "SD_CLASS()": return EToken.ClassDeclaration;
                case "SD_PROPERTY()": return EToken.PropertyDeclaration;
                default: return EToken.INVALID;
            }
        }
    }
}
