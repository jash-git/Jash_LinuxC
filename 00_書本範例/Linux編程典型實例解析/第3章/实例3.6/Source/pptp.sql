use mysql; 
INSERT INTO `user` VALUES ('localhost', 'ppp', PASSWORD( '123' ) , 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N');
INSERT INTO `db` VALUES ('localhost', 'pptp', 'ppp', 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'N', 'Y', 'Y', 'Y');
create database pptp; 
use pptp;
CREATE TABLE `disable` (
  `name` varchar(20) NOT NULL default '',
  `xm` varchar(20) NOT NULL default '',
  `cf` varchar(100) NOT NULL default ''
) TYPE=MyISAM;

CREATE TABLE `login` (
  `pid` varchar(10) NOT NULL default '',
  `name` varchar(20) NOT NULL default '',
  `xm` varchar(20) NOT NULL default '',
  `cf` varchar(100) NOT NULL default ''
) TYPE=MyISAM;

CREATE TABLE `logout` (
  `pid` varchar(10) NOT NULL default '',
  `name` varchar(20) NOT NULL default '',
  `xm` varchar(20) NOT NULL default '',
  `cf` varchar(100) NOT NULL default '',
  `logintime` varchar(20) NOT NULL default '',
  `connecttime` varchar(20) NOT NULL default '',
  `sendbytes` varchar(30) NOT NULL default '',
  `rcvdbytes` varchar(30) NOT NULL default ''
) TYPE=MyISAM;

CREATE TABLE `message` (
  `id` int(10) NOT NULL auto_increment,
  `mess` varchar(200) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM AUTO_INCREMENT=5 ;

CREATE TABLE `user` (
  `name` varchar(20) NOT NULL default '',
  `xm` varchar(20) NOT NULL default '',
  `cf` varchar(30) NOT NULL default ''
) TYPE=MyISAM;

INSERT INTO `user` VALUES ('hsm', '何世明', '教师');
