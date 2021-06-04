divert(-1)
#
# Copyright (c) 1998, 1999 Sendmail, Inc. and its suppliers.
#	All rights reserved.
# Copyright (c) 1983 Eric P. Allman.  All rights reserved.
# Copyright (c) 1988, 1993
#	The Regents of the University of California.  All rights reserved.
#
# By using this file, you agree to the terms and conditions set
# forth in the LICENSE file which can be found at the top level of
# the sendmail distribution.
#
#

#
#  This is a Berkeley-specific configuration file for a specific
#  machine in Electrical Engineering and Computer Sciences at Berkeley,
#  and should not be used elsewhere.   It is provided on the sendmail
#  distribution as a sample only.
#
#  This file is for the primary EECS mail server.
#

divert(0)dnl
VERSIONID(`$Id: mail.eecs.mc,v 8.18 1999/02/07 07:26:04 gshapiro Exp $')
OSTYPE(irix6)dnl
DOMAIN(generic)dnl
MASQUERADE_AS(mail.bme.zju.edu.cn)dnl
MAILER(local)dnl
MAILER(smtp)dnl
define(`DATABASE_MAP_TYPE',`dbm')dnl
define(`confMAX_MESSAGE_SIZE',`15000000')dnl
define(`confTO_QUEUERETURN',`24h')dnl
FEATURE(accept_unresolvable_domains)dnl
FEATURE(accept_unqualified_senders)dnl
FEATURE(`access_db',`dbm /etc/mail/access.db')dnl
FEATURE(`blacklist_recipients')dnl
LOCAL_CONFIG
DDbme.zju.edu.cn

# hosts for which we accept and forward mail (must be in .Berkeley.EDU)
CF mail.bme.zju.edu.cn
FF/etc/sendmail.cw

LOCAL_RULE_0
R< @ $=F . $D . > : $*		$@ $>7 $2		@here:... -> ...
R$* $=O $* < @ $=F . $D . >	$@ $>7 $1 $2 $3		...@here -> ...

R$* < @ $=F . $D . >		$#local $: $1		use UDB
