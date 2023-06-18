using System;
using System.Collections.Generic;
using System.IO;

class CompareTextFiles
{
    static void Main(string[] args)
    {
        string file1Path = "C:\\Users\\micha\\source\\repos\\MikeTat1988\\MikeRep\\TextCompare\\file1.txt";
        string file2Path = "C:\\Users\\micha\\source\\repos\\MikeTat1988\\MikeRep\\TextCompare\\file2.txt";

        if (!File.Exists(file1Path) || !File.Exists(file2Path))
        {
            Console.WriteLine("One or both of the files do not exist.");
            return;
        }

        List<string> differentLines = GetDifferentLines(file1Path, file2Path);

        if (differentLines.Count == 0)
        {
            Console.WriteLine("No differences found between the files.");
        }
        else
        {
            Console.WriteLine("Different lines:");
            foreach (string line in differentLines)
            {
                Console.WriteLine(line);
            }
        }
    }

    static List<string> GetDifferentLines(string file1Path, string file2Path)
    {
        List<string> differentLines = new List<string>();

        string[] file1Lines = File.ReadAllLines(file1Path);
        string[] file2Lines = File.ReadAllLines(file2Path);

        int minLength = Math.Min(file1Lines.Length, file2Lines.Length);

        for (int i = 0; i < minLength; i++)
        {
            if (file1Lines[i] != file2Lines[i])
            {
                differentLines.Add($"Line {i + 1}:\nFile1: {file1Lines[i]}\nFile2: {file2Lines[i]}\n");
            }
        }

        // If one file has more lines than the other, add the extra lines as differences
        if (file1Lines.Length > file2Lines.Length)
        {
            for (int i = minLength; i < file1Lines.Length; i++)
            {
                differentLines.Add($"Line {i + 1}:\nFile1: {file1Lines[i]}\nFile2: ");
            }
        }
        else if (file2Lines.Length > file1Lines.Length)
        {
            for (int i = minLength; i < file2Lines.Length; i++)
            {
                differentLines.Add($"Line {i + 1}:\nFile1: \nFile2: {file2Lines[i]}\n");
            }
        }

        return differentLines;
    }
}
