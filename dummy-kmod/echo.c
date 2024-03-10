#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Menno van der Graaf <mennovandergraaf@hotmail.com>");
MODULE_DESCRIPTION("A simple echo kernel module.");
MODULE_VERSION("1.0");

static char *template = "No template provided. Use template='\"template\"'";
module_param(template, charp, 0000);
MODULE_PARM_DESC(template, "A character string");

static int __init echo_init(void)
{
  printk(KERN_INFO "%s\n", template);
  return 0;
}

static void __exit echo_exit(void)
{
  printk(KERN_INFO "Goodbye, from echo!\n");
}

module_init(echo_init);
module_exit(echo_exit);
