/**
 * 
 * @author Dr. Alnaeli Kent State University, Ohio.
 */
public class Customer
{
	private String ID;
	private double balance;
	/**************************************************************************/
	public Customer()
	{
		ID = "-";
		this.balance = -1;
	}

	public Customer(String iD, double balance)
	{
		ID = iD;
		this.balance = balance;
	}

	public String getID()
	{
		return ID;
	}

	public void setID(String iD)
	{
		ID = iD;
	}

	public double getBalance()
	{
		return balance;
	}

	public void setBalance(double balance)
	{
		this.balance = balance;
	}

	@Override
	public String toString()
	{
		return "Customer [ID=" + ID + ", balance=" + balance + "]";
	}
	
	/**************************************************************************/

}
