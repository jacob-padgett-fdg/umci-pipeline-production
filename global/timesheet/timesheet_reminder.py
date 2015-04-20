#!/usr/bin/python

#----------------------------------------------------------
#
#	Watches the notification table for the
#	problem resolution database, and mails
#	notifications to those that need them.
#
#----------------------------------------------------------

import MySQLdb
import os
import string
import sys
import re
import time

sqlhost="localhost"
sqluser="globalreadwrite"
sqlpass="poilkjbnm"
sqldata="global"
sendmail="/usr/sbin/sendmail"

weekly_day="Friday"
week_end="Saturday"
one_day=86400

dow=time.strftime("%A",time.localtime(time.time()))
current_hour=time.strftime("%H",time.localtime(time.time()))

if (current_hour == "00"): sys.exit()

day_diff=0
today_dow=""
while ((day_diff<(one_day*8))&(today_dow!=week_end)):
	today_dow=time.strftime("%A",time.localtime(time.time() + day_diff))
	week_ending=time.strftime("%m-%d-%Y",time.localtime(time.time() + day_diff))
	week_ending_mysql=time.strftime("%Y-%m-%d",time.localtime(time.time() + day_diff))
	day_diff=day_diff + one_day

if (dow!=weekly_day):
	sys.exit()

timesheet_db=MySQLdb.connect(host=sqlhost,user=sqluser,passwd=sqlpass,db=sqldata)

def mail_user (email,full_name):
	#print "Mailing: " + email
	mailcommand=sendmail + """ -f webmaster@umci.com -F "UMC Timesheet Reminder" -- """ + email
	mailpipe=os.popen(mailcommand,'w')
	mailpipe.write("""From: TIMESHEET REMINDER <webmaster@umci.com>\r\nTo:  """ + email + """\r\nSubject: UMC Timesheet Reminder\r\n""")
	mailpipe.write("""\r
You've asked that a reminder e-mail be sent to you if your timesheet isn't complete\r
by a certain time on Fridays. Well, it appears that the time has come, and your\r
timesheet doesn't appear to be done.\r
\r
Please visit the link below, and complete your timesheet.\r
\r
https://pipeline.umci.com/global/timesheet/\r
\r
\r
Please email webmaster@umci.com if you have any questions.\r
""")
	mailpipe.close()







query="""select * from contacts where umc_emp = 'Y' and email != '' and current = 'Y' and pref_timesheet_reminder_hour = '""" + current_hour + """'""" 
#print query ; sys.exit();

user_list=timesheet_db.cursor(MySQLdb.cursors.DictCursor)
user_list.execute(query)

users=user_list.fetchall()
for user in users:
	#print "Name: " + user["first_name"] + " " + user["last_name"]
	#print str(user["contacts_id"])
	query="""select * from timesheet_index where employee_id = """ + """%d""" % user["contacts_id"] + """ and week_ending = '""" + week_ending_mysql + """'"""
	timesheet_list=timesheet_db.cursor(MySQLdb.cursors.DictCursor)
	timesheet_list.execute(query)
	if (timesheet_list.rowcount != 1):
		mail_user(user["email"],user["first_name"] + " " + user["last_name"])
		continue
	else:
		timesheet_info=timesheet_list.fetchone()
		if (timesheet_info['status']=="new"):
			mail_user(user["email"],user["first_name"] + " " + user["last_name"])
			continue


sys.exit()

