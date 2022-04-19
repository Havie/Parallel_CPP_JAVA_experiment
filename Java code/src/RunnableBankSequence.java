import java.util.ArrayList;

public class RunnableBankSequence implements Runnable
{
	
	private ArrayList<Customer> _custList;
	private int _fromIndex;
	private int _toIndex;
	
	private Counter _countFound;
	
	public static boolean Optimized=true;
	
	/**************************************************************************/
	public RunnableBankSequence(Counter counter, ArrayList<Customer> custList, int fromIndex, int toIndex)
	{
		_custList=custList;
		_fromIndex=fromIndex;
		_toIndex=toIndex;
		_countFound=counter;
	}

	public void run()
	{
		if(Optimized)
			lessBlockedWay();
		else
			easilyBlockedWay();
	}
	
	/**************************************************************************/
	private void easilyBlockedWay()
	{
		for(int i= _fromIndex; i < _toIndex; ++i)
		{
			if(_custList.get(i).getBalance() < 1000)
				_countFound.increment();
		}
	}
	
	private void lessBlockedWay()
	{
		int localCount=0;
		for(int i= _fromIndex; i < _toIndex; ++i)
		{
			if(_custList.get(i).getBalance() < 1000)
				++localCount;
		}
		_countFound.increment(localCount);
	}
	
}
