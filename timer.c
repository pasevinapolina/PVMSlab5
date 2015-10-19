#include <linux/kobject.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Pasevina Polina");
MODULE_DESCRIPTION("Timer for hello world");

static int timer;
struct timer_list hello_timer;
static int active;
static int sleep_time;

static void create_timer(void);
static void timer_func(unsigned long data);

static void timer_func(unsigned long data)
{
    int i;

    for (i = 0; i < data; ++i) {
        printk("Hello world! Interval=%lu\n", jiffies/HZ);
    }
    create_timer();
    printk("\n");

}

static void create_timer()
{
    if (active) {
        del_timer(&hello_timer);
        active = 0;
    }

    if (timer != 0) {
        hello_timer.expires = jiffies + sleep_time * HZ;
        hello_timer.data = timer;
        hello_timer.function = timer_func;
        active = 1;
        add_timer(&hello_timer);
    }
}

static ssize_t timer_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", timer);
}

static ssize_t timer_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%du", &timer);
    create_timer();
    return count;
}

static struct kobj_attribute timer_attribute =
    __ATTR(timer, 0665, timer_show, timer_store);

static struct attribute *attrs[] = {
    &timer_attribute.attr,
    NULL
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static struct kobject *timer_kobj;

static int __init writer_init(void)
{
    int ret;

    active = 0;
    sleep_time = 3;
    init_timer_on_stack(&hello_timer);

    timer_kobj = kobject_create_and_add("kobject_timer", kernel_kobj);
    if (!timer_kobj) {
        return - ENOMEM;
    }

    ret = sysfs_create_group(timer_kobj, &attr_group);
    if (ret) {
        kobject_put(timer_kobj);
        return ret;
    }
    return 0;
}

static void __exit writer_exit(void)
{
    if (active) {
        del_timer(&hello_timer);
        printk(KERN_INFO "Timer removed \n");
    }
    sysfs_remove_group(timer_kobj, &attr_group);
    kobject_put(timer_kobj);
}

module_init(writer_init);
module_exit(writer_exit);
