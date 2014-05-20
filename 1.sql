
CREATE SCHEMA IF NOT EXISTS `db_homework` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `db_homework` ;

-- -----------------------------------------------------
-- Table `db_homework`.`Users`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `db_homework`.`Users` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `isAnonim` tinyint(1) DEFAULT NULL,
  `username` varchar(128) DEFAULT NULL,
  `email` varchar(128) NOT NULL,
  `about` tinytext,
  `name` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`id`),

UNIQUE INDEX `email_UNIQUE` (`email` ASC) )
ENGINE = InnoDB;

-- -----------------------------------------------------
-- Table `db_homework`.`Forums`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `db_homework`.`Forums` (
   `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `short_name` varchar(128) NOT NULL,
  `creator_email` varchar(128) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_shortForumName_idx` (`short_name`),
  KEY `fk_forum_user_idx` (`creator_email`),
  CONSTRAINT `fk_forum_user` FOREIGN KEY (`creator_email`) REFERENCES `Users` (`email`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=689 DEFAULT CHARSET=utf8

-- -----------------------------------------------------
-- Table `db_homework`.`Threads`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `db_homework`.`Threads` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL,
  `isClosed` tinyint(1) NOT NULL,
  `date` datetime NOT NULL,
  `message` text NOT NULL,
  `slug` varchar(255) NOT NULL,
  `isDeleted` tinyint(1) NOT NULL DEFAULT '0',
  `creator_email` varchar(128) NOT NULL,
  `forum_short_name` varchar(128) NOT NULL,
  `likes` int(10) unsigned NOT NULL DEFAULT '0',
  `dislikes` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `fk_thread_user1_idx` (`creator_email`),
  KEY `fk_thread_forum1_idx` (`forum_short_name`),
  CONSTRAINT `fk_thread_forum1` FOREIGN KEY (`forum_short_name`) REFERENCES `Forums` (`short_name`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_thread_user1` FOREIGN KEY (`creator_email`) REFERENCES `Users` (`email`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=662 DEFAULT CHARSET=utf8


-- -----------------------------------------------------
-- Table `db_homework`.`Posts`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `db_homework`.`Posts` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `parent_id` int(11) DEFAULT NULL,
  `is_aproved` tinyint(1) NOT NULL DEFAULT '0',
  `is_highlighted` tinyint(1) NOT NULL DEFAULT '0',
  `is_edited` tinyint(1) NOT NULL DEFAULT '0',
  `is_spam` tinyint(1) NOT NULL DEFAULT '0',
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0',
  `date` datetime NOT NULL,
  `message` text NOT NULL,
  `creator_email` varchar(128) NOT NULL,
  `thread_id` int(10) unsigned NOT NULL,
  `forum` varchar(128) NOT NULL,
  `removed` tinyint(1) NOT NULL DEFAULT '0',
  `likes` int(10) NOT NULL DEFAULT '0',
  `dislikes` int(10) NOT NULL DEFAULT '0',
  `is_deleted_parent` tinyint(4) NOT NULL DEFAULT '0',
  `is_deleted_thread` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `fk_post_user1_idx` (`creator_email`),
  KEY `fk_post_thread1_idx` (`thread_id`),
  CONSTRAINT `fk_post_thread1` FOREIGN KEY (`thread_id`) REFERENCES `Threads` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_post_user1` FOREIGN KEY (`creator_email`) REFERENCES `Users` (`email`) ON DELETE NO ACTION ON UPDATE NO ACTION

ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `db_homework`.`follows`
-- -----------------------------------------------------
CREATE TABLE `follows` (
  `follow_email` varchar(128) NOT NULL,
  `following_email` varchar(128) NOT NULL,
  `deleted` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`follow_email`,`following_email`),
  KEY `fk_user_has_user_user2_idx` (`follow_email`),
  KEY `fk_user_has_user_user1_idx` (`following_email`),
  CONSTRAINT `fk_user_has_user_user1` FOREIGN KEY (`follow_email`) REFERENCES `Users` (`email`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_user_has_user_user2` FOREIGN KEY (`following_email`) REFERENCES `Users` (`email`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8


-- -----------------------------------------------------
-- Table `db_homework`.`subscribe`
-- -----------------------------------------------------
CREATE TABLE `subscribe` (
  `thread_id` int(10) unsigned NOT NULL,
  `user_email` varchar(128) NOT NULL,
  `deleted` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`thread_id`,`user_email`),
  KEY `fk_thread_has_user_user1_idx` (`user_email`),
  KEY `fk_thread_has_user_thread1_idx` (`thread_id`),
  CONSTRAINT `fk_thread_has_user_thread1` FOREIGN KEY (`thread_id`) REFERENCES `Threads` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_thread_has_user_user1` FOREIGN KEY (`user_email`) REFERENCES `Users` (`email`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8

USE `db_homework` ;



