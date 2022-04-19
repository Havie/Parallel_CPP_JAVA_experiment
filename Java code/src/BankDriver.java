
import java.io.File;

import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

public class BankDriver
{

  private final static String ACCOUNT_FILE_NAME= "accounts.txt";
  
	/**************************************************************************/
	public static void main(String[] args) throws Exception
	{
		// create a file with random data by set # of entries
	  //70,000,000
		//createAccountsFile(70000000);
		//100,000
		//createAccountsFile(100000);
	
		ArrayList<Customer> custList= parseTxtFile();

		// Sequential-Run--------------------------------------------------------
		//doSequentialParse(custList);

		// Parallel-Run basic----------------------------------------------------
		//doParallelSequence(custList, false);
		
		// Parallel-Run optimized------------------------------------------------
		doParallelSequence(custList, true);

	}

	/**************************************************************************/
	
	static void createAccountsFile(int count) throws FileNotFoundException
	{
		File accountsFile = new File(ACCOUNT_FILE_NAME);
		PrintStream outFile = new PrintStream(accountsFile);
		Random rand = new Random();
		// needed for making sure that there is no duplication in the IDs
		// ArrayList<String> tmpIDs = new ArrayList<String>();

		// generating random records IDs from XXXX0 to XXXX300000000 (non-repeating) and
		// balances 0 to 1000000
		for (int i = 0; i < count; ++i)
		{
			int tempId;
			tempId = rand.nextInt(300000001);
			// try to generate a random ID with 3 random letters and a number from 0 to
			// 300000000
			String currentID = "" + ((char) ('A' + rand.nextInt(25))) + ((char) ('A' + rand.nextInt(25)))
					+ ((char) ('A' + rand.nextInt(25))) + tempId + ((char) ('A' + rand.nextInt(25)))
					+ ((char) ('A' + rand.nextInt(25))) + ((char) ('A' + rand.nextInt(25)));
			// if (tmpIDs.contains(currentID) == false) {
			// tmpIDs.add(currentID); // insert to the array list in location i
			outFile.println(currentID); // send to file
			// }
			outFile.println(rand.nextInt(1000001) / 100.0); // send to file
		}
		outFile.close();
		
		System.out.println("--new File Was Created!");	
	}
	
	
	private static ArrayList<Customer> parseTxtFile()  throws FileNotFoundException
	{
		long startTime = System.currentTimeMillis();
		ArrayList<Customer> custList = new ArrayList<Customer>();
		// reading the info from the file and storing in the arrayList
		long currentRec = 0;
		System.out.println("Reading the file : / " + ACCOUNT_FILE_NAME);
		Scanner inFile = new Scanner(new File(ACCOUNT_FILE_NAME));
		while (inFile.hasNext())
		{ 
			String currID = inFile.nextLine();
			double currBalance = inFile.nextDouble();

			//Make a new customer from data read
			custList.add(new Customer(currID, currBalance));
			
			if (++currentRec % 30000 == 0)
				System.out.print(">>");
			if (currentRec % 1000000 == 0)
				System.out.println();
			
			//Advance the line to skip past the double we read
			if (inFile.hasNextLine())
				inFile.nextLine();

		} 
		long estimatedTime = System.currentTimeMillis() - startTime;
		System.out.println();
		System.out.println("parsing file run took in miliseconds: " + estimatedTime);
		System.out.println("file has : " + custList.size()*2 + " entries");

		return custList;
	}
	
	/**
	 * @throws FileNotFoundException *************************************************************/
	private static void doSequentialParse(ArrayList<Customer> custList)
	{

		// Counting the balances that are less than 1000$
		int lowBalances = 0;
		// --Sequential-Run--------------------------------------------------------------
		long startTime = System.currentTimeMillis();
		
		for (int i = 0; i < custList.size(); ++i)
			if (custList.get(i).getBalance() < 1000)
				++lowBalances;
		long estimatedTime = System.currentTimeMillis() - startTime;
		System.out.println("Number of accounts with less than 1000$ is: " + lowBalances);
		System.out.println("Sequential run took in miliseconds: " + estimatedTime);
	}
	
	
	private static void doParallelSequence(ArrayList<Customer> custList, boolean optimized) throws InterruptedException
	{
		//a hack to test which way is more optimized
		RunnableBankSequence.Optimized = optimized;

		Counter c = new Counter();
		
		int size= custList.size();
		//Split the work into 4 threads
		int workload = size / 4;
		int range1 = workload;
		int range2 = workload * 2;
		int range3 = workload * 3;
		int range4 = size;
		
		Runnable r1 = new RunnableBankSequence(c, custList, 0 , range1);
		Runnable r2 = new RunnableBankSequence(c, custList, range1, range2);
		Runnable r3 = new RunnableBankSequence(c, custList, range2, range3);
		Runnable r4 = new RunnableBankSequence(c, custList, range3, range4);

		long startTime = System.currentTimeMillis();

		Thread t1 = new Thread(r1);
		Thread t2 = new Thread(r2);
		Thread t3 = new Thread(r3);
		Thread t4 = new Thread(r4);
		
		t1.start();
		t2.start();
		t3.start();
		t4.start();
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		
		long estimatedTime = System.currentTimeMillis() - startTime;
		
		System.out.println("Number of accounts with less than 1000$ is: " + c);
		System.out.println("Parallel run [isOptimized = " + optimized + "] took in miliseconds: " + estimatedTime);
		
	}
	

	
	

	

	

}