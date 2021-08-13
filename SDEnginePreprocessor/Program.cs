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

#if DEBUG
            if (Directory.Exists(outputFileDirectory)) {
                Directory.Delete(outputFileDirectory, true);
            }
            Thread.Sleep(100);  // Wait for the delete.
#endif
            Directory.CreateDirectory(outputFileDirectory);

            // Capture a list of all the header files we can process.
            List<string> allFiles = Directory.GetFiles(args[1], "*.h", SearchOption.AllDirectories).ToList();

            // Indicate the processing is starting.
            Console.WriteLine($"SuburbanDigitalPreprocessor running and attempting to processing {allFiles.Count() } files.");

            List<FileProcessor> reflectedFiles = new List<FileProcessor>();
            int changedFileCount = 0;
            // Process all the files.
            foreach (string filePath in allFiles) {
                FileProcessor processor = new FileProcessor(filePath, outputFileDirectory);
                if (processor.ProcessFile()) {
                    reflectedFiles.Add(processor);
                }
            }

            // Output all the files
            foreach (FileProcessor proc in reflectedFiles) {
                if (!proc.ContainsReflectableContents()) {
                    continue;
                }
                // Only write a new file if the source file had changed. We still have to do the Processing of
                // the file in case other source files that DID change depend on the contents of this once.
                if (proc.HasSourceFileChanged()) {
                    changedFileCount++;
                    proc.GenerateReflectedFile();
                }
            }

            // Indicate the processing is starting.
            Console.WriteLine($"SuburbanDigitalPreprocessor generated reflection files for {changedFileCount} modified sources files.");

            //Indicate we have succeeded.
            Console.WriteLine("SuburbanDigitalPreprocessor successfully executed.");
        }
    }
}
