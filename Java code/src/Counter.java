// Tracks the current value of a counter.
public class Counter
{
	private int count;

	public Counter()
	{
		count = 0;
	}

	//synchronized  solves race condition (will only allow 1 thread to access, blocks for others, until available)
	// - defeats the paralleling to an extent, this becomes the "critical section", a bottleneck,
	// - and your code will only be as fast as this method
	public synchronized void increment()
	{
		++count;
	}
	
	public synchronized void increment(int num)
	{
		count+= num;
	}

	public String toString()
	{
		return "" + count;
	}
}