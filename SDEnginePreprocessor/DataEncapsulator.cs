using System;
using System.Collections.Generic;
using System.Text;

namespace SuburbanDigitalEnginePreprocessor {
    class CPPObject {
        public EReflectionTarget Type;
        public string Name;
        public HashSet<string> ParentClasses;
        public HashSet<string> Properties;


        public CPPObject(EReflectionTarget Type, string Name) {
            this.Type = Type;
            this.Name = Name;
            this.Properties = new HashSet<string>();
            this.ParentClasses = new HashSet<string>();
        }
        public void AddParentClass(string ParentClass) {
            ParentClasses.Add(ParentClass);
        }
        public void AddProperty(string PropertyName) {
            Properties.Add(PropertyName);
        }
        public CPPObject GetCopy() {
            CPPObject copy = new CPPObject(Type, Name);
            foreach(string parent in ParentClasses) {
                copy.AddParentClass(parent);
            }
            foreach(string prop in Properties) {
                copy.AddProperty(prop);
            }
            return copy;
        }
    }
}
