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
victemtable="contacts"
victemlist="issue_res_notify"
issuetable="issue_res_issues"
urlprefix="<a href=https://pipeline.umci.com/global/issue_resolution/index.html?"
email_key="email"
notify_id_key="issue_res_notify_id"
problem_id_key="problem_id"
tomorrow=time.strftime("%Y-%m-%d",time.localtime(time.time() + 86400))
yesterday=time.strftime("%Y-%m-%d",time.localtime(time.time() - 86400))
# Make sure you get the weekly_day set exactly as returned by date's %A
weekly_day="Monday"
dow=time.strftime("%A",time.localtime(time.time()))


if (len(sys.argv)<=1):
	mode="normal"
else:
	mode=sys.argv[1]

#
#	E-mail Message Stuff
#
mailprettyname="UMC Notification System"
mailreturn="webmaster@umci.com"

mailsubject="UMC Issue Resolution Notification"

mailmessagehead='''
<center><font size=+2>UNIVERSITY MECHANICAL Issue Tracking System</font></center><p>

Please visit the link below to proceed with resolving this issue.
<hr>

'''
mailmessagefoot='''
<hr>

This message is to notify you that there is an issue in University
Mechanical's job issue database that requires your attention.
Either the issue is your responsibility, or your collaberation with
the responsible party may be required in order to resolve the issue.

Questions about this e-mail? Read the FAQ section below, or if you have
have technical difficulties or technical questions, send an email to
''' + mailreturn + '''.

<b><font size=+2>Frequently Asked Questions</font></b>
<hr>

Q. Why did I recieve this e-mail?<br>
A. Someone has entered your e-mail address into the University
   Mechanical issue resolution database, and asked that you be
   notified automatically regarding the issue specified.<p>

Q. I already got this notification. Why did I recieve it again.<br>
A. A notification will be sent when the issue is originally created,
   or when you are added to the list of recipients. Another will
   be sent when the issue's due date is approaching and the issue
   has yet to be marked as completed. Finally, notifications will
   be sent periodically for issues that are not marked as complete,
   but the deadline for the issue has come and gone.<p>

'''
mailmessagefoot='''
<hr>

This message is to notify you that there is an issue in University
Mechanical's job issue database that requires your attention.
Either the issue is your responsibility, or your collaberation with
the responsible party may be required in order to resolve the issue.

If you have have technical difficulties or questions about this email
send a message to ''' + mailreturn + '''.'''


# This had better be sendmail, or something *just* like it..
# (and you better be a trusted user)
sendmail="/usr/sbin/sendmail"

conflictdb=MySQLdb.connect(host=sqlhost,user=sqluser,passwd=sqlpass,db=sqldata)

def check_mailings(find_query,unmark_query,addendum_message):
	#jeffmail='jeffb@umci.com'
	find_res=conflictdb.cursor(MySQLdb.cursors.DictCursor)
	find_res.execute(find_query)
	victems=find_res.fetchall()	
	for victem in victems:
		id=victem[problem_id_key]
		prob_id_string='%(id)d' % vars()
		find_status_query="select jobinfo.issue_res_tracking as active,job_name from issue_res_issues,issue_res_category,issue_res_application,jobinfo where issue_res_issues.issue_res_category_id = issue_res_category.issue_res_category_id and issue_res_category.issue_res_application_id = issue_res_application.issue_res_application_id and issue_res_application.jobinfo_id = jobinfo.jobinfo_id and issue_res_issues.problem_id = '" + prob_id_string + "'"
		find_status=conflictdb.cursor(MySQLdb.cursors.DictCursor)
		find_status.execute(find_status_query)
		problem_status=find_status.fetchone()
		if (type(None)!=type(problem_status)):
			if (problem_status["active"] == "Y"):
				#prob_id_string='%(id)d' % vars()
				#print victem
				notify_id_int=victem[notify_id_key]
				notify_id='%(notify_id_int)d' % vars()
				catqry="select * from " + issuetable + " where problem_id = '" + prob_id_string + "'"
				catqry_res=conflictdb.cursor(MySQLdb.cursors.DictCursor)
				catqry_res.execute(catqry)
				cat_record=catqry_res.fetchone()
				cat_num=cat_record["issue_res_category_id"]
				category_id='%(cat_num)d' % vars()
				#reference_url=urlprefix + "glad2=" + prob_id_string + "&glad1=" + category_id + ">View issue</a> (" + problem_status["job_name"] + ")"
				reference_url=urlprefix + "mode=problem_edit&problem_id=" + prob_id_string + ">View issue</a> (" + problem_status["job_name"] + ")"
				reference_url=addendum_message + reference_url
				if victem[email_key] == "":
					print "query for missing email_key: " + find_query;
					print "Error: Contact missing email information. Contact id: " + victem['contacts_id']
				else:
					#print 'mailing user ' + victem[email_key]
					mail_user(victem[email_key],reference_url)
					#mail_user(jeffmail,reference_url)
				if (unmark_query != "none"):
					tmp=conflictdb.cursor(MySQLdb.cursors.DictCursor)
					tmp.execute(unmark_query + notify_id + "'")

def mail_user(address,reference_url):
	mimetag='''------------=_3E260B33.3708D0F9\r\n'''
	mailcommand=sendmail + " -f " + mailreturn + " -F \"" + mailprettyname + "\" -- " + address
	mailglomhead="From: " + mailprettyname + " <" + mailreturn + ">\r\nTo: " + address + "\r\n" + "Subject: " + mailsubject + "\r\n"
	mailglomhead=mailglomhead + '''Content-Type: text/html; charset=utf-8\r\n'''
	mailglomhead=mailglomhead + '''Content-Description: Automatic notification from UMC Mail System\r\n'''
	mailglomhead=mailglomhead + '''Content-Disposition: inline\r\n'''
	mailglomhead=mailglomhead + '''Content-Transfer-Encoding: 8bit\r\n'''
	mailpipe=os.popen(mailcommand,'w')
	mailpipe.write(mailglomhead)
	mailpipe.write(mailmessagehead)
	mailpipe.write(reference_url + "\r\n")
	mailpipe.write(mailmessagefoot)
	mailpipe.close()


def clear_completed_reminders():
	cleardonequery="select * from " + victemlist + "," + issuetable + " where " + victemlist + ".notify_two = 'N' and " + victemlist + ".problem_id = " + issuetable + ".problem_id and " + issuetable + ".completed != '0000-00-00'";
	tmp=clearlist=conflictdb.cursor(MySQLdb.cursors.DictCursor)
	tmp.execute(cleardonequery)
	clear_records=clearlist.fetchall()	
	for record in clear_records:
		zap_id=record["contacts_id"]
		notify_id="%(zap_id)d" % vars()
		clearquery="update " + victemlist + " set notify_two = 'Y' where contacts_id = '" + notify_id + "'"
		tmp=conflictdb.cursor(MySQLdb.cursors.DictCursor)
		tmp.execute(clearquery)

if (mode=="normal"):
	addendum_message="<b>Reason for notification:</b> <i>Recently Created or Altered</i><br>\r\n\r\n"
	#find_query="select * from " + victemlist + "," + victemtable + " where " + victemlist + ".contacts_id = " + victemtable + ".contacts_id and notify_one = 'N'"
	find_query="select contacts.contacts_id as contacts_id,email,problem_id,issue_res_notify_id from " + victemlist + "," + victemtable + " where " + victemlist + ".contacts_id = " + victemtable + ".contacts_id and notify_one = 'N'"
	#print find_query;
	#sys.exit();
	unmark_query="update " + victemlist + " set notify_one = 'Y' where issue_res_notify_id = '"
	check_mailings(find_query,unmark_query,addendum_message)
	sys.exit()

if (mode=="daily"):
	addendum_message="<b>Reason for notification: <font size=+1 color=red><i>*** Due Very Soon, Not Yet Complete ***</i></font></b><br>\r\n\r\n"
	clear_completed_reminders()
	find_query="select contacts.contacts_id as contacts_id,email,problem_id,issue_res_notify_id from " + victemlist + "," + victemtable + "," + issuetable + " where " + victemlist + ".contacts_id = " + victemtable + ".contacts_id and notify_two = 'N' and " + issuetable + ".problem_id = " + victemlist + ".problem_id and " + issuetable + ".due_date = '" + tomorrow + "'"
	#print find_query
	#sys.exit()
	unmark_query="update " + victemlist + " set notify_two = 'Y' where issue_res_notify_id = '"
	check_mailings(find_query,unmark_query,addendum_message)
	if (weekly_day != dow):
		sys.exit()
	else:
		mode="weekly"

if (mode=="weekly"):
	addendum_message="<b>Reason for notification: <font size=+1 color=red>*** *** Due Date Passed, Not Yet Complete *** ***</font></b><br>\r\n\r\n"
	find_query="select contacts.contacts_id as contacts_id,email,problem_id,issue_res_notify_id from " + victemlist + "," + victemtable + "," + issuetable + " where " + victemlist + ".contacts_id = " + victemtable + ".contacts_id and " + issuetable + ".problem_id = " + victemlist + ".problem_id and " + issuetable + ".due_date < '" + yesterday + "' and " + issuetable + ".completed = '0000-00-00'"
	unmark_query="none"
	clear_completed_reminders()
	if (weekly_day != dow):
		print "Waring: Weekly reminder has been run, but doesn't match configured day of week (" + weekly_day + ")\r\n\tContinuing with mailings anyway...\r\n"
	check_mailings(find_query,unmark_query,addendum_message)
	sys.exit()


print "Error: no valid mode \"" + mode + "\" \r\nExitting without processing anything..\r\n"
sys.exit()



