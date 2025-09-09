# Date/Time syncronization

To keep the RTCs of the nodes valid, a host can send time information. To avoid
a 32-Bit problems by sending a single seconds-since-epoch message, the 
information is split into a date and a time message.
All information is Greenwich Mean Time.

Sample code for sending information
'''
   time_t t = time(NULL);
   struct tm tm = *gmtime(&t);

   tdtMessage.date.year=(tm.tm_year + 1900 );
   tdtMessage.date.month=tm.tm_mon + 1;
   tdtMessage.date.day=tm.tm_mday;

   tdtMessage.time.hour=tm.tm_hour;
   tdtMessage.time.min=tm.tm_min;
   tdtMessage.time.sec=tm.tm_sec;
'''

