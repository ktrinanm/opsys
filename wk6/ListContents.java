// Usage: java ListContents directoryname

import java.io.*;
import java.util.*;

public class ListContents 
{
	public static void main(String [] args)
	{
		File f = new File(args[0]);
		String [] dir = f.list();
		Arrays.sort(dir);
		
		for(String s: dir)		// He used normal for loop in class.
		{
			System.out.println(s);
		}
	}
}
