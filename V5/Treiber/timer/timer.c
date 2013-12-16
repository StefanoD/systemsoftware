#include <linux/module.h>
#include <linux/version.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <asm/msr.h> 

static struct timer_list mytimer;
unsigned long ini, end, max = 0, min = 99999999999999, last_jiff;

static void inc_count(unsigned long arg)
{
	unsigned long time;
	rdtscl(end);
	time = end -ini;
	ini = end;
	if(time > max)
	{
		max = time;
	}
	if(time < min)
	{
		min = time;
	}
	printk(KERN_INFO "Differenz TSC: %ld\n", time);
	printk(KERN_INFO "MAX TSC: %ld\n", max);
	printk(KERN_INFO "MIN TSC: %ld\n", min);
	printk(KERN_INFO "inc_count called (%ld)\n", (mytimer.expires - last_jiff));
	last_jiff = mytimer.expires;
	mytimer.expires = jiffies + (2*HZ);	// 2 Sekunden
	add_timer(&mytimer);
}

static int __init ktimer_init(void)
{
	init_timer(&mytimer);	
	mytimer.function = inc_count; //aufzurufende Funktion
	mytimer.data = 0;		//Daten fuer Funktion
	mytimer.expires = jiffies + (2*HZ);		// Zeitpunkt fuer Aufruf
	last_jiff = mytimer.expires;
	rdtscl(ini);
	add_timer(&mytimer);
	return 0;
}

static void __exit	ktimer_exit(void)
{
	if(timer_pending(&mytimer))
	{
		printk(KERN_INFO "Timer ist aktiviert\n");
	}
	if(del_timer_sync(&mytimer))
	{
		printk(KERN_INFO "Timer deaktiviert\n");
	} else {
		printk(KERN_INFO "Kein Timer aktiv\n");
	}
}

module_init(ktimer_init);
module_exit(ktimer_exit);

MODULE_LICENSE("GPL");
