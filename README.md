# Linux Mail Slots

Documentation for the project of Advanced Operating Systems and Virtualization.

**Jacopo Carlini** 
matricola: 1598460

# Index

1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Architecture](#architecture)
4. [Source Code](#source-code)
5. [How To Use](#how-to-use)
6. [Contacts](#contacts)



## Introduction

This is a special device file that is accessible according to FIFO style semantic (via open/close/read/write services), but offering an execution semantic of read/write services such that any segment that is posted to the stream associated with the file is seen as an independent data unit (a message), thus being posted and delivered atomically (all or nothing) and in data separation (with respect to other segments) to the reading threads.

The device file is multi-instance (by having the possibility to manage 256 different instances) so that multiple FIFO style streams (characterized by the above semantic) can be concurrently accessed by active processes/threads.
The device file also supports *ioctl* commands in order to define the runtime behaviour of any I/O session targeting it (such as whether read and/or write operations on a session need to be performed according to blocking or non-blocking rules).

## Installation


You can use the **automatic** way to install:
 
 - To install the module is necessary to open the terminal in the root folder of the project and use these commands:

```
$ ./mount.sh
```

This command installs the module in `/dev/ms0` and the other instances in `/dev/ms1`, etc...
It uses `245` as *Major number* of the module.

 - To remove the module:

```
$ ./unmount.sh
```

 - To remove the module and mount it again with just one command:

```
$ ./remount.sh
```


-	If you want tests this module is possible use some examples in the directory `/user`. You can type this command:
```
$ ./start.sh
```

---

If you prefer install the module **manually** follow these steps:
 1. Enter in `module/` folder and compile with `make` command
```
$ cd module/
$ make
```
2. Use `insmod` command, then use `dmesg` to read the <*Major-number*> and create a node
```
$ sudo insmod mail_slot.ko
$ dmesg
$ sudo mknod /dev/<device-name> c <Major-number> 0
$ sudo chmod 666 /dev/<device-name>
```





## Architecture

The mail slots are implemented by a linked list data structure according to FIFO style semantic: all new messages are put in tail and the messages to read are taken from the head.
Each message is saved in a node that stores the text and the length of the text.
To manage the multiple instances there is a linked list for each instance, so there is an array of linked lists.
These lists are dynamic structures that change them length and contents according to behaviour in runtime. 
Lists are used because they are efficient in adding and removing messages, whereas the array is used because the maximum number of instances is 256, so it is not very expensive use it.  
To use the linked list correctly there is a semaphore (one for reading and one for writing) for each list, so it possible the reading and the writing on the slot in concurrency and without loss.

### Source Code
Below is the source code of the project (`/module` folder) and a brief description of each file and its contents
-   **mail_slot.c**
the main file of the module. There are methods to initialize the module and remove it. The necessary data structures are created. Are present, also, the implementations of the functions of the file operations: open, read, write, release, ioctl.  

-	**mail_struct.h**
files with the data structures of the mail slots: mail (node), mailbox (list) and instances (array of mailbox).

-	**helper.h**
file header containing error handling function and audit functions for debugging.
-	**config.h**
configuration file. You can edit it to change some settings of the project (remember to remount the module).
-	**Makefile**
makefile used for compilation (command `make` from terminal), execution (`make run`) and elimination of executables (`make clean`).


## How To Use
After installing the module, you can `open` a descriptor file associated to the special file and you can use `read` and `write` operations to use the mail slot.  Then you can `close` the descriptor file.
It is possible change the runtime behaviour using `ioctl` commands (input/output control).
You can change:
 *	The maximum number of messages, but it can’t exceed `MAX_MSG`. This value (the default value is **50**) is present in the configuration file (config.h).
*	The maximum length of the text of the message, but it can’t exceed `MAX_MSG_LEN`. This value (the default value is **512**) is present in the configuration file (config.h).
*	Enable/Disable the blocking reading. If it is enable the process/thread waiting until it reads a message. Use **0** disable and **1** to enable.
*	Enable/Disable the blocking writing.  If it is enable the process/thread waiting until it writes a message. Use **0** disable and **1** to enable.

The codes of *ioctl* commands are respectively:
* `#define CMD_MAX_MSG_LEN     1`
* `#define CMD_MAX_MSG         3`
* `#define CMD_BLOCKING_READ   4`
* `#define CMD_BLOCKING_WRITE  5`


## Contacts

 Link of the project: https://github.com/jacopo1395/Linux-Mail-Slots

---
Jacopo Carlini

email: jack1395@hotmail.it

matricola: 1598460

last update: 07/09/2017

