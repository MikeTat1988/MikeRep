using System;
using System.Collections.Generic;
using System.Drawing;
using System.Timers;

public delegate void StormEventHandler(int stormIntensity);

abstract class VisitorGroup
{
    public string GroupType { get; }
    public int Size { get; }

    protected VisitorGroup(string groupType, int size)
    {
        GroupType = groupType;
        Size = size;
    }

    public abstract void ReactToStorm(int stormIntensity);
}

class Family : VisitorGroup
{
    public Family(int size) : base("family", size)
    {
    }

    public override void ReactToStorm(int stormIntensity)
    {
        if (stormIntensity > 2)
        {
            Console.WriteLine($"The family of {Size} takes shelter from the storm of intensity {stormIntensity}.");
        }
        else 
        {
            Console.WriteLine($"The family of {Size} uses umbrellas");
        }
    }
}

class Single : VisitorGroup
{
    public Single(string Name) : base(Name, 1) { }

    public override void ReactToStorm(int stormIntensity)
    {
        if(stormIntensity > 3)
        {
            Console.WriteLine($"{base.GroupType} is singing in the rain");
        }

        else 
        {
            Console.WriteLine($"{base.GroupType} doesn't care");
        }
    }
}

class ElderlyGroup : VisitorGroup
{
    public ElderlyGroup(int size) : base("elderly group", size) { }

    public override void ReactToStorm(int stormIntensity)
    {
         Console.WriteLine($"The elderly group of {Size} is scared and hiding under a bench");
    }

}

class Park
{
    public List<VisitorGroup> visitors = new List<VisitorGroup>();
    private WeatherCentre weatherCentre = new WeatherCentre();

    public Park()
    {
        weatherCentre.StormDetected += AlertVisitors;
    }

    public void Enter(VisitorGroup visitorGroup)
    {
        visitors.Add(visitorGroup);
    }

    public void Leave(VisitorGroup visitorGroup)

    { visitors.Remove(visitorGroup);}

    public void AlertVisitors(int stormIntensity)
    {
        Console.WriteLine($"Storm Intencity of {stormIntensity}");
        foreach (var visitor in visitors)
        {
            visitor.ReactToStorm(stormIntensity);
        }
    }

    public void LeaveFirstOf(string groupType)
    {
        for (int i = 0; i < visitors.Count; i++)
        {
            if (visitors[i].GroupType == groupType)
            {
                visitors.RemoveAt(i);
                break;
            }
        }
    }
}

class WeatherCentre
{
    private static readonly Random random = new Random(); // creates instance of Random class
    private System.Timers.Timer timer;
    private int secondsElapsed;

    public event StormEventHandler? StormDetected;

    public WeatherCentre()
    {
        timer = new System.Timers.Timer(1000); // Check weather every 1 second
        timer.Elapsed += MonitorWeather;
        timer.AutoReset = true;
        timer.Start();
    }
    private void MonitorWeather(object source, ElapsedEventArgs e) // creates a number between 0 to 10
    {
        if (secondsElapsed < 15)
        {
            ++secondsElapsed;

            int stormIntensity = random.Next(0, 11);

            if (stormIntensity > 0)
            {
                StormDetected?.Invoke(stormIntensity);
            }
        }
        else
        {
            timer.Stop();
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        // Instantiate a new park
        Park park = new Park();

        // Add some visitors to the park
        park.Enter(new Family(4));
        park.Enter(new Single("Chad"));
        

        // Keep the console window open long enough to see the results
        // WeatherCentre will automatically start monitoring and triggering alerts
        System.Threading.Thread.Sleep(2000);  // Wait for 2 seconds
        park.Enter(new ElderlyGroup(3));

        System.Threading.Thread.Sleep(2000);  // Wait for 2 seconds
        park.LeaveFirstOf("family");  // The first family leaves

        System.Threading.Thread.Sleep(14000);  // Wait for 2 seconds

        Console.WriteLine("Press any key to exit.");
        Console.ReadKey();
    }
}