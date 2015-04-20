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

sqlhost="reddwarf"
sqluser="jeffb"
sqlpass="bongsrolinge"
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
	issue_id = sys.argv[1]
	issue_id_string=issue_id
	#issue_id_string='%(issue_id)d' % vars()
else:	
	print "ERROR: No issue id specified!"
	sys.exit()
        

# This had better be sendmail, or something *just* like it..
# (and you better be a trusted user)
sendmail="/usr/sbin/sendmail"

it_issuedb=MySQLdb.connect(host=sqlhost,user=sqluser,passwd=sqlpass,db=sqldata)

def mail_them(contact,issue):
	issue_id=issue['issue_id']
	issue_id_string='%(issue_id)d' % vars()
	mimetag='''------------=_3E260B33.3708D0F9\r\n'''
	mailcommand=sendmail + " -f " + mailreturn + " -F \"UMC IT Issue Notifier\" -- " + contact['email']
	mailpipe=os.popen(mailcommand,'w')

	mailpipe.write("From: UMC IT Issue Notifier <webmaster@umci.com>\r\nTo: " + contact['first_name'] + " " + contact['last_name'] + " <" + contact['email'] + ">\r\n" + "Subject: IT Issue Modified (" + issue_id_string + ")\r\n")
	mailpipe.write('''Content-Type: text/html; charset=utf-8\r\n''')
	mailpipe.write('''Content-Description: Automatic notification from UMC Mail System\r\n''')
	mailpipe.write('''Content-Disposition: inline\r\n''')
	mailpipe.write('''Content-Transfer-Encoding: 8bit\r\n''')
	
	requested_completion=issue['requested_completion']
	rcm=requested_completion.month
	rcd=requested_completion.day
	rcy=requested_completion.year
	rcms='%(rcm)d' % vars()
	rcds='%(rcd)d' % vars()
	rcys='%(rcy)d' % vars()
	requested_completion_string=rcms + "-" + rcds + "-" + rcys


	expected_completion=issue['expected_completion']
	ecm=expected_completion.month
	ecd=expected_completion.day
	ecy=expected_completion.year
	ecms='%(ecm)d' % vars()
	ecds='%(ecd)d' % vars()
	ecys='%(ecy)d' % vars()
	expected_completion_string=ecms + "-" + ecds + "-" + ecys

	action_completion=issue['expected_completion_date']
	acm=action_completion.month
	acd=action_completion.day
	acy=action_completion.year
	acms='%(acm)d' % vars()
	acds='%(acd)d' % vars()
	acys='%(acy)d' % vars()
	action_completion_string=acms + "-" + acds + "-" + acys
	
	if issue['us_or_them']=="them":
		issue['us_or_them']="<font color=red><b>USERS (that's you)</b></font>"
	else:
		issue['us_or_them']="<font color=green>IT</font>"
		
	message="""
	<h2>IT Issue """ + issue_id_string + """ Updated</h2><hr>
	<a href=http://pipeline.umci.com/global/mis_issue_request/?mode=issue_edit&issue_id=""" + issue_id_string + """><font color=blue>Load this issue</font></a><br>
	<b>New issue information:</b>
	<table border=1>
	<tr><td>
		Subject
	</td><td>
		""" + issue['name'] + """
	</td></tr>

	<tr><td>
		Original Description
	</td><td>
		""" + issue['description'] + """ 
	</td></tr>

	<tr><td>
		Requested Completion Date
	</td><td>
		""" + requested_completion_string + """
	</td></tr>

	<tr><td>
		Expected Completion Date
	</td><td>
		""" + expected_completion_string + """
	</td></tr>

	<tr><td>
		Next Action Date
	</td><td>
		""" + action_completion_string + """
	</td></tr>

	<tr><td>
		Status
	</td><td>
		""" + issue['status'] + """
	</td></tr>

	<tr><td>
		Notes
	</td><td>
		""" + issue['notes'] + """
	</td></tr>

	<tr><td>
		Next Action
	</td><td>
		""" + issue['action_required'] + """
	</td></tr>

	<tr><td>
		Next Action Responsibility
	</td><td>
		""" + issue['us_or_them'] + """
	</td></tr>

	</table>
	"""


	mailpipe.write(message + "\r\n")
	mailpipe.close()


def mail_us(contact,issue):
	issue_id=issue['issue_id']
	issue_id_string='%(issue_id)d' % vars()
	mimetag='''------------=_3E260B33.3708D0F9\r\n'''
	mailcommand=sendmail + " -f " + mailreturn + " -F \"UMC IT Issue Notifier\" -- " + contact['email']
	mailpipe=os.popen(mailcommand,'w')

	mailpipe.write("From: UMC IT Issue Notifier <webmaster@umci.com>\r\nTo: " + contact['first_name'] + " " + contact['last_name'] + " <" + contact['email'] + ">\r\n" + "Subject: IT Issue Modified (" + issue_id_string + ")\r\n")
	mailpipe.write('''Content-Type: text/html; charset=utf-8\r\n''')
	mailpipe.write('''Content-Description: Automatic notification from UMC Mail System\r\n''')
	mailpipe.write('''Content-Disposition: inline\r\n''')
	mailpipe.write('''Content-Transfer-Encoding: 8bit\r\n''')
	
	requested_completion=issue['requested_completion']
	rcm=requested_completion.month
	rcd=requested_completion.day
	rcy=requested_completion.year
	rcms='%(rcm)d' % vars()
	rcds='%(rcd)d' % vars()
	rcys='%(rcy)d' % vars()
	requested_completion_string=rcms + "-" + rcds + "-" + rcys


	expected_completion=issue['expected_completion']
	ecm=expected_completion.month
	ecd=expected_completion.day
	ecy=expected_completion.year
	ecms='%(ecm)d' % vars()
	ecds='%(ecd)d' % vars()
	ecys='%(ecy)d' % vars()
	expected_completion_string=ecms + "-" + ecds + "-" + ecys


	action_completion=issue['expected_completion_date']
	acm=action_completion.month
	acd=action_completion.day
	acy=action_completion.year
	acms='%(acm)d' % vars()
	acds='%(acd)d' % vars()
	acys='%(acy)d' % vars()
	action_completion_string=acms + "-" + acds + "-" + acys

	if issue['us_or_them']=="them":
		issue['us_or_them']="<font color=green>USERS</font>"
	else:
		issue['us_or_them']="<font color=red><b>IT</b></font>"
		
	message="""
	<h2>IT Issue """ + issue_id_string + """ Updated</h2><hr>
	<a href=http://pipeline.umci.com/global/mis_issue_tracking/?mode=issue_edit&issue_id=""" + issue_id_string + """><font color=blue>Load this issue</font></a><br>
	<b>New issue information:</b>
	<table border=1>
	<tr><td>
		Subject
	</td><td>
		""" + issue['name'] + """
	</td></tr>

	<tr><td>
		Original Description
	</td><td>
		""" + issue['description'] + """ 
	</td></tr>

	<tr><td>
		Requested Completion Date
	</td><td>
		""" + requested_completion_string + """
	</td></tr>

	<tr><td>
		Expected Completion Date
	</td><td>
		""" + expected_completion_string + """
	</td></tr>

	<tr><td>
		Next Action Date
	</td><td>
		""" + action_completion_string + """
	</td></tr>

	<tr><td>
		Status
	</td><td>
		""" + issue['status'] + """
	</td></tr>

	<tr><td>
		Notes
	</td><td>
		""" + issue['notes'] + """
	</td></tr>

	<tr><td>
		Next Action
	</td><td>
		""" + issue['action_required'] + """
	</td></tr>

	<tr><td>
		IT Notes
	</td><td>
		""" + issue['it_notes'] + """
	</td></tr>

	<tr><td>
		Next Action Responsibility
	</td><td>
		""" + issue['us_or_them'] + """
	</td></tr>

	</table>
	"""


	mailpipe.write(message + "\r\n")
	mailpipe.close()


creator_notified=0

query=""" select * from mis_issue_index,mis_issue_history where mis_issue_index.issue_id = '""" + issue_id_string + """' and mis_issue_index.issue_id = mis_issue_history.issue_id and current_hist_item = 'Y' and email_sent = 'N'"""
issues=it_issuedb.cursor(MySQLdb.cursors.DictCursor)
issues.execute(query)

if (issues.rowcount > 1):
	print "ERROR: more than 1 current item for that issue.. that's not good!"
	sys.exit()

if (issues.rowcount < 1):
	print "ERROR: no current item, or mail already sent!"
	sys.exit()

issue_list=issues.fetchall()
issue=issue_list[0]


issue_history_id=issue['issue_history_id']
issue_history_id_string='%(issue_history_id)d' %vars()

query="select * from mis_issue_concerned_parties,contacts where issue_history_id = '" + issue_history_id_string + "' and mis_issue_concerned_parties.contacts_id = contacts.contacts_id"
issues.execute(query)
userlist=issues.fetchall()


for user in userlist:
	if user['contacts.contacts_id']==issue['creator']:
		creator_notified=1
	if user['contacts.contacts_id']!=issue['entered_by']:
		if user['us_or_them']=='us':
			mail_us(user,issue)
		else:
			mail_them(user,issue)

if creator_notified==0:
	print "creator not seen.. might want to look him/her up and notify him/her"

query="update mis_issue_history set email_sent = 'Y' where issue_history_id = '" + issue_history_id_string + "'"
issues.execute(query)

sys.exit()



