using System;
using System.Collections.Generic;
using System.Text;

namespace SuburbanDigitalEnginePreprocessor {
    class FProperty {
        public string Name;
        private HashSet<string> Modifiers;
        private Dictionary<string, string> ModifierValues;

        public FProperty() {
            Modifiers = new HashSet<string>();
            ModifierValues = new Dictionary<string, string>();
        }
        public FProperty(string PropertyName, string MacroLine) : this() {
            Name = PropertyName;
            int firstParen = MacroLine.IndexOf('(');
            int lastParen = MacroLine.LastIndexOf(')');
            if (firstParen + 1 < lastParen) {
                string adjustedMacroLine = MacroLine.Substring(firstParen + 1, lastParen - firstParen - 1);
                string[] arguments = adjustedMacroLine.Split(',');
                foreach (string arg in arguments) {
                    string trimmedArg = arg.Trim();
                    if (trimmedArg.Contains('=')) {
                        string[] splitArgs = trimmedArg.Split('=');
                        Modifiers.Add(splitArgs[0].Trim());
                        ModifierValues.Add(splitArgs[0].Trim(), splitArgs[1].Trim());
                    } else {
                        Modifiers.Add(trimmedArg);
                    }
                }
            }
        }
        public string GetMacroInternals() {
            return $"{Name}, \"{GetInspectorName()}\", {HideInInspector()}, \"{GetCategory()}\"";
        }
        public string GetInspectorName() {
            if (Modifiers.Contains("OverrideInspectorName")) {
                string containedName = ModifierValues["OverrideInspectorName"];
                containedName = containedName.Replace("(", "");
                containedName = containedName.Replace(")", "");
                containedName = containedName.Replace("\"", "");
                return containedName;
            } else {
                string adjustedName = Name;
                // Strip out leading b if this was a boolean value. Carryover from Unreal Engine habits.
                if (Name.StartsWith("b")) {
                    if (Name.Length > 1) {
                        if (Char.IsUpper(Name[1])) {
                            adjustedName = Name.Substring(1);
                        }
                    }
                }

                string splitString = "";
                bool prevWasLower = false;
                for (int i = 0; i < adjustedName.Length; i++) {
                    if(char.IsUpper(adjustedName[i]) && prevWasLower) {
                        splitString += " ";
                        splitString += adjustedName[i];
                        prevWasLower = false;
                    } else {
                        prevWasLower = true;
                        splitString += adjustedName[i];
                    }
                }

                return splitString;
            }
        }
        public string HideInInspector() {
            return Modifiers.Contains("InspectorHidden").ToString().ToLower();
        }
        public string GetCategory() {
            string category = "Default";
            if(ModifierValues.ContainsKey("Category")) {
                category = ModifierValues["Category"];
            }
            category = category.Replace("(", "");
            category = category.Replace(")", "");
            category = category.Replace("\"", "");
            return category;
        }
    }
    class CPPObject {
        public EReflectionTarget Type;
        public string Name;
        public HashSet<string> ParentClasses;
        public Dictionary<string, FProperty> Properties;


        public CPPObject(EReflectionTarget Type, string Name) {
            this.Type = Type;
            this.Name = Name;
            this.Properties = new Dictionary<string, FProperty>();
            this.ParentClasses = new HashSet<string>();
        }
        public void AddParentClass(string ParentClass) {
            ParentClasses.Add(ParentClass);
        }
        public void AddProperty(FProperty Property) {
            Properties.Add(Property.Name, Property);
        }
        public CPPObject GetCopy() {
            CPPObject copy = new CPPObject(Type, Name);
            foreach (string parent in ParentClasses) {
                copy.AddParentClass(parent);
            }
            foreach (FProperty prop in Properties.Values) {
                copy.AddProperty(prop);
            }
            return copy;
        }
    }
}
