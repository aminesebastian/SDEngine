using System;
using System.Collections.Generic;
using System.Text;

namespace SuburbanDigitalEnginePreprocessor {
    enum FTypeMemoryType {
        VALUE, REFERENCE, POINTER
    }

    class FProperty : FReflectableField {
        public FProperty(string PropertyName, string MacroLine) : base() {
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
        public override string GetMacroInternals() {
            return $"{Name}, \"{GetInspectorName()}\",  \"{GetCategory()}\", {HideInInspector()}";
        }
        private string GetInspectorName() {
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
                        if (char.IsUpper(Name[1])) {
                            adjustedName = Name.Substring(1);
                        }
                    }
                }

                string splitString = "";
                bool prevWasLower = false;
                for (int i = 0; i < adjustedName.Length; i++) {
                    if (char.IsUpper(adjustedName[i]) && prevWasLower) {
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
        private string HideInInspector() {
            return Modifiers.Contains("InspectorHidden").ToString().ToLower();
        }
    }
    class FFunction : FReflectableField {
        public bool ReturnValueConst { get; private set; }
        public bool ConstMethod { get; private set; }
        public string ReturnType { get; private set; }
        public FTypeMemoryType ReturnMemoryType { get; private set; }
        public Dictionary<string, FFunctionParameter> Parameters { get; private set; }

        public FFunction(string FunctionLine, string MacroLine) : base() {
            Parameters = new Dictionary<string, FFunctionParameter>();

            string funcLine = FunctionLine;
            if (funcLine.StartsWith("const")) {
                ReturnValueConst = true;
                funcLine = funcLine.Substring(6).Trim();
            }

            // Split the function declaration after the const (if there was a const).
            string[] functionSplit = funcLine.Split(' ', StringSplitOptions.RemoveEmptyEntries);

            // Capture the return type.
            ReturnType = functionSplit[0];
            funcLine = funcLine.Substring(ReturnType.Length).Trim();

            // Clean the return type and determine the return memory type.
            if (ReturnType.Contains("*")) {
                ReturnMemoryType = FTypeMemoryType.POINTER;
                ReturnType = ReturnType.Replace("*", "");
            } else if(ReturnType.Contains("&")) {
                ReturnMemoryType = FTypeMemoryType.REFERENCE;
                ReturnType = ReturnType.Replace("&", "");
            } else {
                ReturnMemoryType = FTypeMemoryType.VALUE;
            }

            // Captures the function name.
            int parentIndex = funcLine.IndexOf('(');
            Name = funcLine.Substring(0, parentIndex);
            funcLine = funcLine.Substring(parentIndex);

            funcLine = funcLine.Replace("(", "").Replace(")", "").Replace("{", "").Replace("}", "").Replace(";", "").Trim();

            if (funcLine.EndsWith("const")) {
                ConstMethod = true;
                funcLine = funcLine.Substring(0, funcLine.Length - 5);
            }

            string[] parameters = funcLine.Split(',', StringSplitOptions.RemoveEmptyEntries);
            foreach (string param in parameters) {
                FFunctionParameter paramObject = new FFunctionParameter(param.Trim());
                Parameters.Add(paramObject.Name, paramObject);
            }
        }
        public override string GetMacroInternals() {
            StringBuilder output = new StringBuilder();
            output.Append($"{Name}, {ReturnValueConst.ToString().ToLower()}, {ReturnType}, {(int)ReturnMemoryType}, {ConstMethod.ToString().ToLower()}");

            if (Parameters.Count > 0) {
                output.Append(", ");
                foreach (FFunctionParameter param in Parameters.Values) {
                    output.Append("{" + param.GetMacroInternals() + "}, ");
                }
                // If we had parameters for this method, ensure we don't include the final comma.
                return output.ToString().Substring(0, output.Length - 2);
            } else {
                return output.ToString();
            }
        }
    }
    class FFunctionParameter : FReflectableField {
        public bool Const { get; private set; }
        public FTypeMemoryType PassType { get; private set; }
        public string ParameterType { get; private set; }
        public FFunctionParameter(string ParameterString) {
            string paramLine = ParameterString;
            if (paramLine.StartsWith("const")) {
                Const = true;
                paramLine = paramLine.Substring(6).Trim();
            }

            string[] splitParam = paramLine.Split(' ', StringSplitOptions.RemoveEmptyEntries);
            if (splitParam[0].Contains("&") || splitParam[1].Contains("&")) {
                PassType = FTypeMemoryType.REFERENCE;
            } else if (splitParam[0].Contains("*") || splitParam[1].Contains("*")) {
                PassType = FTypeMemoryType.POINTER;
            } else {
                PassType = FTypeMemoryType.VALUE;
            }

            ParameterType = splitParam[0].Replace("&", "").Replace("*", "");
            Name = splitParam[1].Replace("&", "").Replace("*", "");
        }
        public override string GetMacroInternals() {
            return $"{Name}, {ParameterType}, {Const}, {(int)PassType}";
        }
    }
    class FReflectableField {
        public string Name { get; protected set; }
        protected HashSet<string> Modifiers;
        protected Dictionary<string, string> ModifierValues;

        public FReflectableField() {
            Modifiers = new HashSet<string>();
            ModifierValues = new Dictionary<string, string>();
        }
        public virtual string GetMacroInternals() {
            return $"{Name}";
        }
        public string GetCategory() {
            string category = "Default";
            if (ModifierValues.ContainsKey("Category")) {
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
        public List<FFunction> Functions;

        public CPPObject(EReflectionTarget Type, string Name) {
            this.Type = Type;
            this.Name = Name;
            Properties = new Dictionary<string, FProperty>();
            Functions = new List<FFunction>();
            ParentClasses = new HashSet<string>();
        }
        public void AddParentClass(string ParentClass) {
            ParentClasses.Add(ParentClass);
        }
        public void AddProperty(FProperty Property) {
            Properties.Add(Property.Name, Property);
        }
        public void AddFunction(FFunction Function) {
            Functions.Add(Function);
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
