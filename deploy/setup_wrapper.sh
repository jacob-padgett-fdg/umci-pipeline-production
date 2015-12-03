#!/bin/bash

#
# change notification email to proper recipient
#
notify=josh@itsecureadmin.com
this_host=$(hostname)

deploybranch=$1

if [[ "${deploybranch}" -ne "master" || "${deploybranch}" -ne "development" || "${deploybranch}" -ne "staging" || "${deploybranch}" -ne "production" ]]
then
  echo "no valid deploy branch specified"
  exit 1;
fi

#
# set / verify JAVA_HOME
#
if [[ -n JAVA_HOME ]]
then
  # JAVA_HOME set 
  echo "JAVA_HOME:  ${JAVA_HOME}"
elif [[ -d /usr/lib/jvm/java-6-openjdk ]]
then
  export JAVA_HOME=/usr/lib/jvm/java-6-openjdk
else
  echo "No JAVA_HOME known."
  exit 1;
fi

#
# perform ant deploy
# - git pull
# - liquibase DB update
#
results=$(ant -Ddeploybranch=${deploybranch} syncDB)
if [[ $? -gt 0 ]]
then
  echo "${results}" | mail -s "ant deploy failed on ${this_host}" ${notify}
  exit 1;
fi

exit 0;
