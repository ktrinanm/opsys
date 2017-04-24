// Usage: java FileClass. Lists filenames 

import java.io.*;
import java.util.*;

public class FileClass
{
	public static void main(String [] args) throws IOException
	{
		File f = new File(args[0]);
		
		if(f.exists())	//Could use a try-catch here instead
		{
			System.out.println("File found");
			System.out.println("File name: " + f.getName());
		}
		else
		{
			System.out.println("File not found");
			System.out.println("Making file: " + f.getName());
			f.createNewFile();
		}

		//similar methods for reading and writing.
		System.out.println("This file's absolute path is: " + f.getAbsolutePath());

		File p = f.getParentFile();
		System.out.println("This file's parent is: " + (p != null ? p.getName() : "null"));

		System.out.println("The file is " + (f.isDirectory() ? "" : "not ") + "a directory.");

		System.out.println("This file is " + (f.isFile() ? "" : "not ") + "a file.");

		System.out.println("This file " + (f.canExecute() ? "can " : "cannot ") + "execute.");

		System.out.println("This file " + (f.canRead() ? "can " : "cannot ") + "be read.");

		System.out.println("This file " + (f.canWrite() ? "can " : "cannot ") + "be written to.");

		System.out.println("This file was last modified: " + new Date(f.lastModified()));

		String [] files =  f.list();

		if(files != null)
		{
			for(String file: files)
			{
				System.out.println(file);
			}
		}

		f.mkdir();
		System.out.println("A directory was made using this file's path");

		File fc2 = new File("renamed.txt");
		if(f.renameTo(fc2))
		{
			System.out.println("After renaming, the file " + (f.exists() ? "no longer " : "") + "exists");
			if(!f.exists())
			{
				System.out.println("The new filename is: " + fc2.getName());
			}
		}
		else
		{
			System.out.println("Renaming did not succeed.");
		}

		if(fc2.setReadOnly())
		{
			System.out.println("This file was set to read only.");
		}
		else
		{
			System.out.println("This file cannot be set to read only.");
		}

		if(fc2.delete())
		{
			System.out.println("File " + fc2.getName() + " was Deleted.");
		}
	}
}
