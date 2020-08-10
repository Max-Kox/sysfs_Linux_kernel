#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/string.h>

static int user_number = 0;
static int errors;
static struct kobject *converting_kobj;

static const char *numbers_str[10] = {
	"zero", "one", "two", "three", "four",
	"five", "six", "seven", "eight", "nine"
};

static ssize_t input_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	int ret;

	ret = __sysfs_match_string(numbers_str, 10, buf);
	if (ret < 0) {
		errors++;
		return ret;
	}

	user_number = ret;

	return count;
}

static ssize_t output_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%d\n", user_number);
}

static ssize_t errors_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%d\n", errors);
}

static struct kobj_attribute input_attribute =
	__ATTR(input, 0664, NULL, input_store);
static struct kobj_attribute output_attribute =
	__ATTR(output, 0664, output_show, NULL);
static struct kobj_attribute errors_attribute =
	__ATTR(errors, 0664, errors_show, NULL);

static struct attribute *attrs[] = {
	&input_attribute.attr,
	&output_attribute.attr,
	&errors_attribute.attr,
	NULL,
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
	.attrs = attrs,
};

static int __init sysfs_attrs_init(void)
{
	int retval;

	converting_kobj = kobject_create_and_add("kobject_example", kernel_kobj);
	if (!converting_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(converting_kobj, &attr_group);
	if (retval)
		kobject_put(converting_kobj);

	return retval;
}

static void __exit sysfs_attrs_exit(void)
{
	kobject_put(converting_kobj);
}

module_init(sysfs_attrs_init);
module_exit(sysfs_attrs_exit);

MODULE_AUTHOR("Max Kokhan");
MODULE_DESCRIPTION("Sysfs attributes");
MODULE_LICENSE("GPL");
