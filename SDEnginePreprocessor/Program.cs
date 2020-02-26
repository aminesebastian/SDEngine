using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;

namespace SuburbanDigitalEnginePreprocessor {
    class Program {
        static void Main(string[] args) {
            // Capture the output file directory.
            string outputFileDirectory = args[0];
            if(Directory.Exists(outputFileDirectory)) {
                Directory.Delete(outputFileDirectory, true);
            }
            Thread.Sleep(100);
            Directory.CreateDirectory(outputFileDirectory);

            // Capture a list of all the header files we can process.
            List<string> allFiles = Directory.GetFiles(args[1], "*.h", SearchOption.AllDirectories).ToList();

            // Indicate the processing is starting.
            Console.WriteLine($"SuburbanDigitalPreprocessor running and attempting to processing {allFiles.Count() } Files.");

            List<FileProcessor> reflectedFiles = new List<FileProcessor>();

            // Process all the files.
            foreach(string filePath in allFiles) {
                FileProcessor processor = new FileProcessor(filePath, outputFileDirectory);
                if(processor.ProcessFile()) {
                    reflectedFiles.Add(processor);
                }
            }

            // Output all the files
            foreach(FileProcessor proc in reflectedFiles) {
                proc.GenerateReflectedFile();
            }

            //Indicate we have succeeded.
            Console.WriteLine("SuburbanDigitalPreprocessor successfully executed.");
        }
    }
}
