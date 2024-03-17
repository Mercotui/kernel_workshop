#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/seq_buf.h>

// Declare Module Metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Menno van der Graaf <mennovandergraaf@hotmail.com>");
MODULE_DESCRIPTION("A simple echo kernel module.");
MODULE_VERSION("1.0");

// Declare Module Parameters
static char *template = "No template provided. Use template='\"template\"'";
module_param(template, charp, 0000);
MODULE_PARM_DESC(template, "A template containing ${variables} to expand");

// Declare Output Buffer
DECLARE_SEQ_BUF(output_buff, 10000);
DECLARE_SEQ_BUF(variable_value_buff, 1000);

/**
 * get_variable_value() - Return the value for name
 *
 * This function is not reentrant, the returned buffer is reused for every call.
 * Make sure to use the returned buffer before calling the function again.
 */
const char *get_variable_value(const char *name)
{
  seq_buf_clear(&variable_value_buff);

  if (strcmp(name, "HZ") == 0) {
    seq_buf_printf(&variable_value_buff, "%d", HZ);
  } else if (strcmp(name, "USER_HZ") == 0) {
    seq_buf_printf(&variable_value_buff, "%d", USER_HZ);
  } else {
    seq_buf_puts(&variable_value_buff, "'unknown variable'");
  }

  return seq_buf_str(&variable_value_buff);
}

/**
 * expand_variable() - Take variable name between {} and return its value
 */
const char *expand_variable(char **input)
{
  if (**input != '{') {
    return "'found $ without matching {'";
  }
  (*input)++;

  const char *name = strsep(input, "}");
  if (*input == NULL) {
    return "'found ${ without matching }'";
  }
  if (*name == '\0') {
    return "'found empty ${}'";
  }

  return get_variable_value(name);
}

/**
 * expand_variables() - Search string for variables and expand them
 */
void expand_variables(char *input, struct seq_buf *output)
{
  char *rest = input;
  while (rest != NULL) {
    const char *token = strsep(&rest, "$");
    seq_buf_puts(output, token);

    if (rest != NULL) {
      seq_buf_puts(output, expand_variable(&rest));
    }
  }
}

static int __init echo_init(void)
{
  expand_variables(template, &output_buff);

  seq_buf_do_printk(&output_buff, KERN_INFO);
  return 0;
}

static void __exit echo_exit(void)
{
  printk(KERN_INFO "Goodbye, from echo!\n");
}

module_init(echo_init);
module_exit(echo_exit);
