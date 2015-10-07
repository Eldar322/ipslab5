/*
*	for learn kobject
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

static unsigned long sleep_time = 3000;

static ssize_t show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%lu\n", sleep_time);
}

static ssize_t store(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t count)
{
  unsigned long tmp;

  if (kstrtoul(buf, 10, &tmp) == -EINVAL) {
    return -EINVAL;
  }
  sleep_time = tmp;

	return count;
}

static struct kobj_attribute sc_attrb =
	__ATTR(sleep_time, 0666, show, store);

static struct kobject *kobj;

static int __init repeat_hello_init(void)
{
	int ret;

	/* create a dir in sys/ */
	kobj = kobject_create_and_add("repeat_hello", NULL);
	if (!kobj)
		return - ENOMEM;

	/* create a attribute file in kobj_example */
 	ret = sysfs_create_file(kobj, &sc_attrb.attr);
	if (ret)
		goto attr_file_failed;
	return 0;

attr_file_failed:
	kobject_put(kobj);
	return ret;
}

static void __exit repeat_hello_exit(void)
{
	sysfs_remove_file(kobj, &sc_attrb.attr);
	kobject_put(kobj);
}

module_init(repeat_hello_init);
module_exit(repeat_hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Gamisoniya Eldar");
