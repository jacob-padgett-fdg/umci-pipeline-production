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

tomorrow=time.strftime("%Y-%m-%d",time.localtime(time.time() + 86400))
yesterday=time.strftime("%Y-%m-%d",time.localtime(time.time() - 86400))
day_of_week=time.strftime("%A",time.localtime(time.time()))

#
#	E-mail Message Stuff
#
mailprettyname="IT Issue Notifier"
mailreturn="webmaster@umci.com"

if (len(sys.argv)>1):
	if (sys.argv[1]=="new"):
		mailsubject="**NEW ISSUE** ITEM JUST SUBMITTED"
	else:
		mailsubject="**NEW ISSUE** IT Issue Tracker"
else:	
	mailsubject="**NEW ISSUE** IT Issue Tracker"
        

mailmessagehead='''
<center><font size=+2>IT Issue Tracking</font></center><p>

There is one or more new issue. Do something about it, or I'll mail you again in an hour or so.
<ul><a href=http://pipeline.umci.com/global/mis_issue_tracking/>Go here to fix this</a></ul>
<hr>

'''
mailmessagefoot='''
<hr>
'''


# This had better be sendmail, or something *just* like it..
# (and you better be a trusted user)
sendmail="/usr/sbin/sendmail"

it_issuedb=MySQLdb.connect(host=sqlhost,user=sqluser,passwd=sqlpass,db=sqldata)

def mail_user(address,message):
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
	mailpipe.write(message + "\r\n")
	mailpipe.write(mailmessagefoot)
	mailpipe.close()


#mail_user ('jeffb@umci.com','This is a test message');

query=''' select * from mis_issue_index,contacts where creator = contacts_id and status = 'initialized' '''
new_issues=it_issuedb.cursor(MySQLdb.cursors.DictCursor)
new_issues.execute(query)

if (new_issues.rowcount < 1):
	sys.exit()


message="""Summary of new issues waiting to be accepted:<p>"""
issue_list=new_issues.fetchall()
for issue in issue_list:
	issue_id=issue['issue_id']
	issue_id_string='%(issue_id)d' % vars()
	message=message + """
	<li>""" + """(<a href=http://pipeline.umci.com/global/mis_issue_tracking/index.html?mode=issue_edit&issue_id=""" + issue_id_string + """>""" + issue_id_string + """</a>) By: """ + issue['login_name'] + """ - """ + issue['name'] + """</li>"""

uslist=it_issuedb.cursor(MySQLdb.cursors.DictCursor)
uslist.execute("select * from mis_issue_uslist,contacts where mis_issue_uslist.contacts_id = contacts.contacts_id")
mail_list=uslist.fetchall()
for contact in mail_list:
	if (contact['pref_uslist_exclude'] != 'Y'):
		mail_user (contact['email'],message)







sys.exit()



