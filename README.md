# Linux-Mail-Slots
special device file that is accessible according to FIFO style semantic (via open/close/read/write services), but offering an execution semantic of read/write services such that any segment that is posted to the stream associated with the file is seen as an independent data unit (a message), thus being posted and delivered atomically (all or nothing) and in data separation (with respect to other segments) to the reading threads. 

