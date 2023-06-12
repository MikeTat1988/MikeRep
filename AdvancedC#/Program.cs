using System;

public class NotificationService
{
    public delegate void NotificationDelegate (string prameter);
    public event NotificationDelegate? NotificationEvent;

    public void NotifySubscribers(string message)
    {
        NotificationEvent?.Invoke(message);
    }

    public void Sub(Subscriber subscriber)
    {
        NotificationEvent += subscriber.HandleNotification;
    }

    public void Unsub(Subscriber subscriber)
    {
        NotificationEvent -= subscriber.HandleNotification;
    }
}

public class Subscriber
{
    public NotificationService mynote { get; set; }
    public Subscriber(NotificationService notification)
    {
        mynote = notification;
    }

    public void HandleNotification(string message)
    {
        Console.WriteLine(message);
    }
}

class Program
{
    static void Main(string[] args)
    {
        NotificationService notificationService = new NotificationService();

        Subscriber subscriber1 = new Subscriber(notificationService);
        Subscriber subscriber2 = new Subscriber(notificationService);
        Subscriber subscriber3 = new Subscriber(notificationService);

        notificationService.Sub(subscriber1);
        notificationService.Sub(subscriber2);
        notificationService.Sub(subscriber3);


        notificationService.NotifySubscribers("hello");

        Console.ReadLine();
    }

}

