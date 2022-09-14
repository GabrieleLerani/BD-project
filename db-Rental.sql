-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema Noleggio_film
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema Noleggio_film
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `Noleggio_film` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci ;
USE `Noleggio_film` ;

-- -----------------------------------------------------
-- Table `Noleggio_film`.`Utente`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Utente` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Utente` (
  `ID` INT NOT NULL AUTO_INCREMENT,
  `Username` VARCHAR(45) NOT NULL,
  `Password` VARCHAR(45) NOT NULL,
  `Ruolo` ENUM('impiegato', 'manager', 'cliente') NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `Username_UNIQUE` (`Username` ASC) VISIBLE)
ENGINE = InnoDB
AUTO_INCREMENT = 66
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Impiegato`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Impiegato` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Impiegato` (
  `CF` VARCHAR(45) NOT NULL,
  `Nome` VARCHAR(45) NOT NULL,
  `TitoloStudio` ENUM('laurea', 'diploma') NULL DEFAULT NULL,
  `Telefono` VARCHAR(45) NULL DEFAULT NULL,
  `ID` INT NOT NULL,
  PRIMARY KEY (`CF`),
  UNIQUE INDEX `ID_UNIQUE` (`ID` ASC) VISIBLE,
  UNIQUE INDEX `Telefono_UNIQUE` (`Telefono` ASC) VISIBLE,
  CONSTRAINT `fk_Impiegato_1`
    FOREIGN KEY (`ID`)
    REFERENCES `Noleggio_film`.`Utente` (`ID`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Manager`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Manager` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Manager` (
  `Nome` VARCHAR(45) NOT NULL,
  `CFManager` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Nome`, `CFManager`),
  UNIQUE INDEX `CFManager_UNIQUE` (`CFManager` ASC) VISIBLE,
  CONSTRAINT `fk_Manager_1`
    FOREIGN KEY (`CFManager`)
    REFERENCES `Noleggio_film`.`Impiegato` (`CF`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Centro`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Centro` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Centro` (
  `IDCentro` INT NOT NULL AUTO_INCREMENT,
  `NomeManager` VARCHAR(45) NULL DEFAULT NULL,
  `CFManager` VARCHAR(45) NOT NULL,
  `Telefono` VARCHAR(45) NULL DEFAULT NULL,
  `Email` VARCHAR(100) NULL DEFAULT NULL,
  `Via` VARCHAR(45) NOT NULL,
  `Numero` SMALLINT NOT NULL,
  `CAP` INT(5) UNSIGNED ZEROFILL NOT NULL,
  PRIMARY KEY (`IDCentro`),
  UNIQUE INDEX `IDCentro_UNIQUE` (`IDCentro` ASC) VISIBLE,
  UNIQUE INDEX `CFManager_UNIQUE` (`CFManager` ASC) VISIBLE,
  UNIQUE INDEX `CAP_UNIQUE` (`CAP` ASC) VISIBLE,
  UNIQUE INDEX `NomeManager_UNIQUE` (`NomeManager` ASC) VISIBLE,
  UNIQUE INDEX `Email_UNIQUE` (`Email` ASC) VISIBLE,
  UNIQUE INDEX `Telefono_UNIQUE` (`Telefono` ASC) VISIBLE,
  INDEX `fk_Centro_1_idx` (`NomeManager` ASC, `CFManager` ASC) VISIBLE,
  CONSTRAINT `fk_Centro_1`
    FOREIGN KEY (`NomeManager` , `CFManager`)
    REFERENCES `Noleggio_film`.`Manager` (`Nome` , `CFManager`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
AUTO_INCREMENT = 3
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Settore`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Settore` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Settore` (
  `IDSettore` INT NOT NULL AUTO_INCREMENT,
  `IDCentro` INT NOT NULL,
  `Posizione` INT NOT NULL DEFAULT '0',
  PRIMARY KEY (`IDSettore`, `IDCentro`, `Posizione`),
  INDEX `fk_Settore_1_idx` (`IDCentro` ASC) VISIBLE,
  CONSTRAINT `fk_Settore_1`
    FOREIGN KEY (`IDCentro`)
    REFERENCES `Noleggio_film`.`Centro` (`IDCentro`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
AUTO_INCREMENT = 4
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Film`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Film` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Film` (
  `Titolo` VARCHAR(45) NOT NULL,
  `Regista` VARCHAR(45) NOT NULL,
  `CostoNoleggio` DOUBLE NULL DEFAULT NULL,
  `AnnoUscita` DATE NULL DEFAULT NULL,
  `CopieDisponibili` INT NULL DEFAULT NULL,
  `Tipo` ENUM('classico', 'ultima uscita') NULL DEFAULT NULL,
  `Centro` INT NOT NULL,
  `Settore` INT NOT NULL,
  `Posizione` INT NOT NULL,
  PRIMARY KEY (`Titolo`, `Regista`, `Centro`, `Settore`, `Posizione`),
  INDEX `fk_Film_1_idx` (`Centro` ASC, `Settore` ASC, `Posizione` ASC) VISIBLE,
  CONSTRAINT `fk_Film_1`
    FOREIGN KEY (`Centro` , `Settore` , `Posizione`)
    REFERENCES `Noleggio_film`.`Settore` (`IDCentro` , `IDSettore` , `Posizione`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Attore`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Attore` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Attore` (
  `Nome` VARCHAR(45) NOT NULL,
  `Titolo` VARCHAR(45) NOT NULL,
  `Regista` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Nome`),
  INDEX `fk_Attore_1_idx` (`Titolo` ASC, `Regista` ASC) VISIBLE,
  CONSTRAINT `fk_Attore_1`
    FOREIGN KEY (`Titolo` , `Regista`)
    REFERENCES `Noleggio_film`.`Film` (`Titolo` , `Regista`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Cliente`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Cliente` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Cliente` (
  `CF` VARCHAR(45) NOT NULL,
  `Nome` VARCHAR(45) NOT NULL,
  `Cognome` VARCHAR(45) NOT NULL,
  `DataNascita` DATE NULL DEFAULT NULL,
  `LuogoNascita` VARCHAR(45) NULL DEFAULT NULL,
  `ID` INT NULL DEFAULT NULL,
  PRIMARY KEY (`CF`),
  UNIQUE INDEX `Nome_UNIQUE` (`Nome` ASC) VISIBLE,
  UNIQUE INDEX `Cognome_UNIQUE` (`Cognome` ASC) VISIBLE,
  UNIQUE INDEX `ID_UNIQUE` (`ID` ASC) VISIBLE,
  CONSTRAINT `fk_Cliente_1`
    FOREIGN KEY (`ID`)
    REFERENCES `Noleggio_film`.`Utente` (`ID`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Noleggio`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Noleggio` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Noleggio` (
  `ID` INT NOT NULL AUTO_INCREMENT,
  `DataInizio` DATE NOT NULL,
  `DataFine` DATE NOT NULL,
  PRIMARY KEY (`ID`))
ENGINE = InnoDB
AUTO_INCREMENT = 23
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`EffettuaNoleggio`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`EffettuaNoleggio` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`EffettuaNoleggio` (
  `Cliente` VARCHAR(45) NOT NULL,
  `Noleggio` INT NOT NULL,
  `Terminato` TINYINT(1) NOT NULL,
  PRIMARY KEY (`Cliente`, `Noleggio`),
  INDEX `fk_EffettuaNoleggio_2_idx` (`Noleggio` ASC) VISIBLE,
  CONSTRAINT `fk_EffettuaNoleggio_1`
    FOREIGN KEY (`Cliente`)
    REFERENCES `Noleggio_film`.`Cliente` (`CF`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_EffettuaNoleggio_2`
    FOREIGN KEY (`Noleggio`)
    REFERENCES `Noleggio_film`.`Noleggio` (`ID`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`FilmCorrispondeNoleggio`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`FilmCorrispondeNoleggio` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`FilmCorrispondeNoleggio` (
  `Titolo` VARCHAR(45) NOT NULL,
  `Regista` VARCHAR(45) NOT NULL,
  `Noleggio` INT NOT NULL,
  `Centro` INT NOT NULL,
  PRIMARY KEY (`Titolo`, `Regista`, `Noleggio`, `Centro`),
  INDEX `fk_FilmCorrispondeNoleggio_2_idx` (`Noleggio` ASC) VISIBLE,
  INDEX `fk_FilmCorrispondeNoleggio_1` (`Titolo` ASC, `Regista` ASC, `Centro` ASC) VISIBLE,
  CONSTRAINT `fk_FilmCorrispondeNoleggio_1`
    FOREIGN KEY (`Titolo` , `Regista` , `Centro`)
    REFERENCES `Noleggio_film`.`Film` (`Titolo` , `Regista` , `Centro`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_FilmCorrispondeNoleggio_2`
    FOREIGN KEY (`Noleggio`)
    REFERENCES `Noleggio_film`.`Noleggio` (`ID`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`FilmRemake`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`FilmRemake` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`FilmRemake` (
  `TitoloRemake` VARCHAR(45) NOT NULL,
  `RegistaRemake` VARCHAR(45) NOT NULL,
  `TitoloOriginale` VARCHAR(45) NOT NULL,
  `RegistaOriginale` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`TitoloRemake`, `RegistaRemake`),
  INDEX `fk_FilmRemake_1_idx` (`TitoloOriginale` ASC, `RegistaOriginale` ASC) VISIBLE,
  CONSTRAINT `fk_FilmRemake_1`
    FOREIGN KEY (`TitoloOriginale` , `RegistaOriginale`)
    REFERENCES `Noleggio_film`.`Film` (`Titolo` , `Regista`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`ImpiegatoGestioneNoleggio`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`ImpiegatoGestioneNoleggio` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`ImpiegatoGestioneNoleggio` (
  `Impiegato` VARCHAR(45) NOT NULL,
  `Noleggio` INT NOT NULL,
  PRIMARY KEY (`Impiegato`, `Noleggio`),
  INDEX `fk_ImpiegatoGestioneNoleggio_2_idx` (`Noleggio` ASC) VISIBLE,
  CONSTRAINT `fk_ImpiegatoGestioneNoleggio_1`
    FOREIGN KEY (`Impiegato`)
    REFERENCES `Noleggio_film`.`Impiegato` (`CF`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_ImpiegatoGestioneNoleggio_2`
    FOREIGN KEY (`Noleggio`)
    REFERENCES `Noleggio_film`.`Noleggio` (`ID`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Impiego`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Impiego` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Impiego` (
  `IDImpiego` INT NOT NULL AUTO_INCREMENT,
  `CFImpiegato` VARCHAR(45) NOT NULL,
  `DataInizio` DATE NOT NULL,
  `DataFine` DATE NULL DEFAULT NULL,
  `Carica` ENUM('cassiere', 'commesso') NULL DEFAULT NULL,
  `IDCentro` INT NULL DEFAULT NULL,
  PRIMARY KEY (`IDImpiego`, `CFImpiegato`, `DataInizio`),
  INDEX `fk_Impiego_1_idx` (`CFImpiegato` ASC) VISIBLE,
  INDEX `fk_Impiego_2_idx` (`IDCentro` ASC) VISIBLE,
  CONSTRAINT `fk_Impiego_1`
    FOREIGN KEY (`CFImpiegato`)
    REFERENCES `Noleggio_film`.`Impiegato` (`CF`),
  CONSTRAINT `fk_Impiego_2`
    FOREIGN KEY (`IDCentro`)
    REFERENCES `Noleggio_film`.`Centro` (`IDCentro`))
ENGINE = InnoDB
AUTO_INCREMENT = 19
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`PostaElettronica`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`PostaElettronica` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`PostaElettronica` (
  `Email` VARCHAR(100) NOT NULL,
  `Cliente` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Email`),
  UNIQUE INDEX `Email_UNIQUE` (`Email` ASC) VISIBLE,
  INDEX `fk_PostaElettronica_1_idx` (`Cliente` ASC) VISIBLE,
  CONSTRAINT `fk_PostaElettronica_1`
    FOREIGN KEY (`Cliente`)
    REFERENCES `Noleggio_film`.`Cliente` (`CF`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Telefono`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Telefono` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Telefono` (
  `Cliente` VARCHAR(45) NOT NULL,
  `Numero` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Numero`),
  UNIQUE INDEX `Numero_UNIQUE` (`Numero` ASC) VISIBLE,
  INDEX `fk_Telefono_1_idx` (`Cliente` ASC) VISIBLE,
  CONSTRAINT `fk_Telefono_1`
    FOREIGN KEY (`Cliente`)
    REFERENCES `Noleggio_film`.`Cliente` (`CF`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `Noleggio_film`.`Turno`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`Turno` ;

CREATE TABLE IF NOT EXISTS `Noleggio_film`.`Turno` (
  `CFImpiegato` VARCHAR(45) NOT NULL,
  `Giorno` ENUM('lunedi', 'martedi', 'mercoledi', 'giovedi', 'venerdi', 'sabato') NOT NULL,
  `OraInizio` TIME NULL DEFAULT NULL,
  `OraFine` TIME NULL DEFAULT NULL,
  `IDImpiego` INT NULL DEFAULT NULL,
  PRIMARY KEY (`CFImpiegato`, `Giorno`),
  INDEX `fk_Turno_2_idx` (`IDImpiego` ASC) VISIBLE,
  CONSTRAINT `fk_Turno_1`
    FOREIGN KEY (`IDImpiego`)
    REFERENCES `Noleggio_film`.`Impiego` (`IDImpiego`),
  CONSTRAINT `fk_Turno_2`
    FOREIGN KEY (`CFImpiegato`)
    REFERENCES `Noleggio_film`.`Impiegato` (`CF`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;

USE `Noleggio_film` ;

-- -----------------------------------------------------
-- Placeholder table for view `Noleggio_film`.`noleggi_utenti`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Noleggio_film`.`noleggi_utenti` (`ID` INT, `DataInizio` INT, `DataFine` INT, `Titolo` INT, `Regista` INT, `Cliente` INT, `Nome` INT, `Centro` INT, `Terminato` INT);

-- -----------------------------------------------------
-- Placeholder table for view `Noleggio_film`.`report_per_centro`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Noleggio_film`.`report_per_centro` (`CFImpiegato` INT, `IDImpiego` INT, `IDCentro` INT, `DataInizio` INT, `DataFine` INT, `Settimane_1` INT, `Settimane_2` INT, `Giorno` INT, `OreLavorate` INT);

-- -----------------------------------------------------
-- Placeholder table for view `Noleggio_film`.`settori_liberi`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Noleggio_film`.`settori_liberi` (`IDSettore` INT, `IDCentro` INT, `Posizione` INT);

-- -----------------------------------------------------
-- procedure end_rental
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`end_rental`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `end_rental`(in rental_id int)
BEGIN
	declare title varchar(45);
    declare producer varchar(45);
    declare center varchar(45);
    
    declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
	start transaction;
	
    if(rental_id not in (select Noleggio from `EffettuaNoleggio`)) then
		signal sqlstate '45000' set message_text = "Nessun noleggio corrispondente";
	
    else
    
		if (select count(*) from `EffettuaNoleggio` where Terminato = 1 and Noleggio = rental_id)>0 then
			signal sqlstate '45000' set message_text = "Il noleggio è già stato terminato";
		
        else
		update `EffettuaNoleggio` 
			set Terminato = 1 where Noleggio = rental_id;
    
		select Titolo,Regista,Centro into title,producer,center from `noleggi_utenti` where ID = rental_id;
    
		update `Film`
			set CopieDisponibili = CopieDisponibili + 1 where Titolo = title and Regista = producer and Centro = center; 
    
		update `Noleggio`
			set DataFine = curdate();
		
        delete from `EffettuaNoleggio` where Terminato = 1 and Noleggio = rental_id;
        delete from `FilmCorrispondeNoleggio` where Noleggio = rental_id;
        delete from `Noleggio` where ID = rental_id;
        
        end if;
    
	end if;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure get_actors
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`get_actors`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_actors`(in title varchar(45),in producer varchar(45))
BEGIN
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
		if(title not in (select Titolo from `Attore`)) then
			signal sqlstate '45000' set message_text = "Il titolo inserito non ha attori registrati";
        end if;
        select Nome from `Attore`  
		where Titolo = title and 
			Regista = producer;
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure get_employees
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`get_employees`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_employees`()
BEGIN
	declare exit handler for sqlexception
    
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
	start transaction;
    if(select count(*) from `Impiegato`) = 0 then
		signal sqlstate '45000' set message_text = "Non ci sono impiegati registrati";
	else select * from `Impiegato` where ID in (select ID from Utente where Ruolo = 'Impiegato');
    end if;
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure get_employment_id
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`get_employment_id`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_employment_id`(in cf varchar(45))
BEGIN
	declare exit handler for sqlexception
    
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
	start transaction;
		if(cf not in (select CFImpiegato from Impiego)) then
			signal sqlstate '45000' set message_text = "All'impiegato indicato non corrisponde ancora alcun impiego";
		else select IDImpiego from Impiego where CFImpiegato = cf;
		end if;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure get_positions
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`get_positions`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_positions`(in center_id int)
BEGIN
	declare exit handler for sqlexception
    
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
	
    if(center_id not in (select IDCentro from `Noleggio_film`.`settori_liberi`)) then
		signal sqlstate '45000' set message_text = "Il centro inserito non esiste o non ha settori liberi";

    else select IDSettore,Posizione from `Noleggio_film`.`settori_liberi` where IDCentro = center_id;
    end if;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure get_remake
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`get_remake`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_remake`()
BEGIN
	
    declare exit handler for sqlexception
    
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
	start transaction;
    if(select count(*) from `FilmRemake`) = 0 then
		signal sqlstate '45000' set message_text = "Non ci sono film remake registrati";
	else select * from `FilmRemake`;
    end if;
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure get_report
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`get_report`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `get_report`(in center_id int)
BEGIN
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
	start transaction;
		if(center_id not in(select IDCentro from report_per_centro)) then
			signal sqlstate '45000' set message_text = "Per il centro inserito non è possibile ottenere report";
		else
			(select CFImpiegato,DataInizio,DataFine,Settimane_1 as Settimane,Giorno,OreLavorate from Noleggio_film.report_per_centro where curdate()>DataFine and IDCentro = center_id)
			union
			(select CFImpiegato,DataInizio,curdate() as DataFine,Settimane_2 as Settimane,Giorno,OreLavorate from Noleggio_film.report_per_centro where curdate()<DataFine and IDCentro = center_id);
		end if;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_actor
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_actor`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_actor`(in act_name varchar(45),in title varchar(45),in producer varchar(45))
BEGIN
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
		insert into `Attore` values(act_name,title,producer);
    commit;
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_email
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_email`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_email`(in cf varchar(45),in email varchar(100))
BEGIN
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
	
	insert into `PostaElettronica` values(email,cf);
    
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_employee
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_employee`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_employee`(in cf varchar(45),in birthname varchar(45),in qualification enum('diploma','laurea'),in phone varchar(45),in username varchar(45),in pass varchar(45))
BEGIN
	declare tempID int;
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
		-- Insert user 
		insert into	`Utente`(Username,Password,Ruolo) values (username,pass,'impiegato');
        
        -- Retrieve id for just added client
        select ID from Utente as U 
        where username=U.username and password=U.Password into tempID;
        
        -- Insert into cliente new user with his id
        insert into `Impiegato` values(cf,birthname,qualification,phone,tempID);
		
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_employment
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_employment`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_employment`(in cf varchar(45),in start_date date,in end_date date, in type_of_work enum('cassiere','commesso'),in center int)
BEGIN
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
	
	insert into `Impiego`(CFImpiegato,DataInizio,DataFine,Carica,IDCentro) values(cf,start_date,end_date,type_of_work,center);
    
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_film
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_film`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_film`(in titolo varchar(45),in regista varchar(45),in costo int,in anno date, in copie int,in tipo enum('classico','ultima uscita'),in centro int,in settore int,in posizione int)
BEGIN
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
		insert into `Film` values(titolo,regista,costo,anno,copie,tipo,centro,settore,posizione);
    commit;
    
    
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_phone
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_phone`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_phone`(in cf varchar(45),in phone varchar(45))
BEGIN
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
	
	insert into `Telefono` values(cf,phone);
    
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_remake
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_remake`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_remake`(in remake_title varchar(45),in remake_producer varchar(45),in title varchar(45),in producer varchar(45))
BEGIN
	declare exit handler for sqlexception
    
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
	start transaction;
    
    insert into `FilmRemake` values(remake_title,remake_producer,title,producer);
	
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_rental
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_rental`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_rental`(in user_id int,in filmtitle varchar(45),in producer varchar(45),in end_date date,in center int)
BEGIN
	 -- declare tempID int;
    declare temp_cf varchar(45);
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
    
		if(end_date<curdate()) then
			signal sqlstate '45000' set message_text = "La data inserita non è valida";
        end if;
		
        if(user_id in (select ID from `Cliente`)) then
			insert into `Noleggio`(DataInizio,DataFine) values (curdate(),end_date);
            
            select CF from `Cliente` where ID = user_id into temp_cf;
            insert into `EffettuaNoleggio` values(temp_cf,last_insert_id(),'0');
            
            if((filmtitle not in (select Titolo from `Film` where Centro = center)) 
				and	(producer not in (select Regista from `Film` where Centro = center))) then
				
                signal sqlstate '45000' set message_text = "Il film inserito non corrisponde al centro inserito";

			else insert into `FilmCorrispondeNoleggio` values(filmtitle,producer,last_insert_id(),center);
            end if;
            
            if(center not in (select Centro from `Film`)) then
				signal sqlstate '45000' set message_text = "Il film inserito non si trova in nessun centro";
            
            else update `Film` 
					set CopieDisponibili = CopieDisponibili - 1
                    where Titolo = filmtitle and Regista = producer and Centro = center;
			
            end if;
            
        else signal sqlstate '45000' set message_text = "Cliente non registrato nel sistema";
		end if;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_user
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_user`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_user`(in cf varchar(45),in birthname varchar(45),in lastname varchar(45), in birthday date,in birthplace varchar(45),in username varchar(45),in pass varchar(45))
BEGIN
	declare tempID int;
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
		-- Insert user 
		insert into	`Utente`(Username,Password,Ruolo) values (username,pass,'cliente');
        
        -- Retrieve id for just added client
        select ID from Utente as U 
        where username=U.username and password=U.Password into tempID;
        
        -- Insert into cliente new user with his id
        insert into `Cliente` values(cf,birthname,lastname,birthday,birthplace,tempID);
		
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure insert_workshift
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`insert_workshift`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_workshift`(in cf varchar(45),in dayofshift enum('lunedi','martedi','mercoledi','giovedi','venerdi','sabato'),in start_time time, in end_time time,in employment_id int)
BEGIN
	
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
	
	insert into `Turno` values(cf,dayofshift,start_time,end_time,employment_id);
    
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure list_active_rentals
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`list_active_rentals`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `list_active_rentals`(in cf varchar(45))
BEGIN
	declare temp_count int;
    declare exit handler for sqlexception
    
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
	start transaction;
    
    if(cf not in (select Cliente from `EffettuaNoleggio`)) then
		signal sqlstate '45000' set message_text = "Il cliente non ha noleggi registrati";
	end if;
    
    
    (select count(*) from `noleggi_utenti` 
		where curdate() < DataFine and ID in (select Noleggio 
												from EffettuaNoleggio 
                                                where Cliente = cf)) into temp_count;
    
	if temp_count = 0 then
		signal sqlstate '45000' set message_text = "Tutti i noleggi di questo cliente sono scaduti";
	
    else	 
		select ID,DataInizio,DataFine,Titolo,Regista  
        from `noleggi_utenti` 
		where curdate() < DataFine and ID in (select Noleggio 
												from EffettuaNoleggio 
                                                where Cliente = cf);
	end if;
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure list_available_film
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`list_available_film`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `list_available_film`()
BEGIN
	declare exit handler for sqlexception
    
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;

	SELECT DISTINCT `Titolo`,`Regista`,`CostoNoleggio`,`AnnoUscita` 
	FROM `Film`
    WHERE `Film`.`CopieDisponibili`>0;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure list_expired_rentals
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`list_expired_rentals`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `list_expired_rentals`(in center_id int)
BEGIN
	declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
		if (center_id not in (select Centro from `noleggi_utenti` 
							where curdate() > DataFine and Terminato = 0)) then
				signal sqlstate '45000' set message_text = "Non ci sono noleggi in questo centro";
        
        else
		select ID,DataInizio,DataFine,Titolo,Regista,Cliente,Nome  
        from `noleggi_utenti` 
		where curdate() > DataFine and Terminato = 0 and Centro = center_id;
		end if;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure list_shifts
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`list_shifts`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `list_shifts`(in cf varchar(45))
BEGIN
	declare exit handler for sqlexception
    
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
	
    if(cf not in (select CFImpiegato from `Turno`)) then
		signal sqlstate '45000' set message_text = "L'impiegato non ha turni inseriti";
	else select Giorno,OraInizio,OraFine from `Turno` where CFImpiegato = cf;
	end if;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure login
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`login`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `login`(in var_username varchar(45), in var_pass varchar(100),out var_role int)
BEGIN
declare var_user_role ENUM('impiegato','cliente','manager');
    
    select `Ruolo` from `Utente`
		where `Username` = var_username
        and `Password` = var_pass
        into var_user_role;
        
	-- See the corresponding enum in the client
		if var_user_role = 'manager' then
			set var_role = 1;
		elseif var_user_role = 'impiegato' then
			set var_role = 2;
        elseif var_user_role = 'cliente' then
			set var_role = 3;    
		else
			set var_role = 4;
		end if;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure move_employee
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`move_employee`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `move_employee`(in cf varchar(45),in new_center int)
BEGIN
	declare exit handler for sqlexception
    
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level read committed;
    
	if(cf in (select CFImpiegato from `Impiego`)) then 
		update `Impiego` set IDCentro = new_center where CFImpiegato = cf;
	else signal sqlstate '45000' set message_text = "L'impiegato non è registrato";
 
    end if;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure register_center
-- -----------------------------------------------------

USE `Noleggio_film`;
DROP procedure IF EXISTS `Noleggio_film`.`register_center`;

DELIMITER $$
USE `Noleggio_film`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `register_center`(in man_name varchar(45),in man_cf varchar(45),in phone varchar(45),in email varchar(45),in via varchar(45),in numero varchar(45),in cap varchar(45))
BEGIN
declare exit handler for sqlexception
    begin
		rollback;
        resignal;
	end;
    
    set transaction isolation level repeatable read;
    start transaction;
	
	insert into `Centro`(NomeManager,CFManager,Telefono,Email,Via,Numero,CAP) values(man_name,man_cf,phone,email,via,numero,cap);
    
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- View `Noleggio_film`.`noleggi_utenti`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`noleggi_utenti`;
DROP VIEW IF EXISTS `Noleggio_film`.`noleggi_utenti` ;
USE `Noleggio_film`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `Noleggio_film`.`noleggi_utenti` AS select `N`.`ID` AS `ID`,`N`.`DataInizio` AS `DataInizio`,`N`.`DataFine` AS `DataFine`,`FCN`.`Titolo` AS `Titolo`,`FCN`.`Regista` AS `Regista`,`EN`.`Cliente` AS `Cliente`,`C`.`Nome` AS `Nome`,`FCN`.`Centro` AS `Centro`,`EN`.`Terminato` AS `Terminato` from (((`Noleggio_film`.`EffettuaNoleggio` `EN` join `Noleggio_film`.`Noleggio` `N`) join `Noleggio_film`.`FilmCorrispondeNoleggio` `FCN`) join `Noleggio_film`.`Cliente` `C`) where ((`EN`.`Noleggio` = `FCN`.`Noleggio`) and (`EN`.`Noleggio` = `N`.`ID`) and (`EN`.`Cliente` = `C`.`CF`));

-- -----------------------------------------------------
-- View `Noleggio_film`.`report_per_centro`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`report_per_centro`;
DROP VIEW IF EXISTS `Noleggio_film`.`report_per_centro` ;
USE `Noleggio_film`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `Noleggio_film`.`report_per_centro` AS select `T`.`CFImpiegato` AS `CFImpiegato`,`T`.`IDImpiego` AS `IDImpiego`,`report`.`IDCentro` AS `IDCentro`,`report`.`DataInizio` AS `DataInizio`,`report`.`DataFine` AS `DataFine`,`report`.`Settimane_1` AS `Settimane_1`,`report`.`Settimane_2` AS `Settimane_2`,`T`.`Giorno` AS `Giorno`,timediff(`T`.`OraFine`,`T`.`OraInizio`) AS `OreLavorate` from ((select `Noleggio_film`.`Impiego`.`CFImpiegato` AS `CFImpiegato`,`Noleggio_film`.`Impiego`.`IDCentro` AS `IDCentro`,round(((to_days(`Noleggio_film`.`Impiego`.`DataFine`) - to_days(`Noleggio_film`.`Impiego`.`DataInizio`)) / 7),0) AS `Settimane_1`,round(((to_days(curdate()) - to_days(`Noleggio_film`.`Impiego`.`DataInizio`)) / 7),0) AS `Settimane_2`,`Noleggio_film`.`Impiego`.`DataInizio` AS `DataInizio`,`Noleggio_film`.`Impiego`.`DataFine` AS `DataFine`,`Noleggio_film`.`Impiego`.`IDImpiego` AS `IDImpiego` from `Noleggio_film`.`Impiego`) `report` join `Noleggio_film`.`Turno` `T` on(((`report`.`CFImpiegato` = `T`.`CFImpiegato`) and (`report`.`IDImpiego` = `T`.`IDImpiego`))));

-- -----------------------------------------------------
-- View `Noleggio_film`.`settori_liberi`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `Noleggio_film`.`settori_liberi`;
DROP VIEW IF EXISTS `Noleggio_film`.`settori_liberi` ;
USE `Noleggio_film`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `Noleggio_film`.`settori_liberi` AS select `S`.`IDSettore` AS `IDSettore`,`S`.`IDCentro` AS `IDCentro`,`S`.`Posizione` AS `Posizione` from `Noleggio_film`.`Settore` `S` where exists(select `F`.`Centro`,`F`.`Settore`,`F`.`Posizione` from `Noleggio_film`.`Film` `F` where ((`S`.`IDCentro` = `F`.`Centro`) and (`S`.`IDSettore` = `F`.`Settore`) and (`S`.`Posizione` = `F`.`Posizione`))) is false;
SET SQL_MODE = '';
DROP USER IF EXISTS login;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'login' IDENTIFIED BY 'Login1234!';

GRANT EXECUTE ON procedure `Noleggio_film`.`login` TO 'login';
SET SQL_MODE = '';
DROP USER IF EXISTS impiegato;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'impiegato' IDENTIFIED BY 'Impiegato1234!';

SET SQL_MODE = '';
DROP USER IF EXISTS cliente;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'cliente' IDENTIFIED BY 'Cliente1234!';

SET SQL_MODE = '';
DROP USER IF EXISTS manager;
SET SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
CREATE USER 'manager' IDENTIFIED BY 'Manager1234!';


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
USE `Noleggio_film`;

DELIMITER $$

USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Utente_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Utente_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`Utente`
FOR EACH ROW
BEGIN
	if(new.Username in (select Username from Utente)) then
		signal sqlstate '45000' set message_text = "Username già in uso"; 
    end if;
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Manager_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Manager_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`Manager`
FOR EACH ROW
BEGIN
	if new.CFManager not in (select CF from Impiegato) then
		signal sqlstate '45000' set message_text = "Questo impiegato non è registrato";
	end if;
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Settore_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Settore_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`Settore`
FOR EACH ROW
BEGIN
	if
    (select count(*)
	from Centro as C
	where new.IDCentro = C.IDCentro) = 0 then
			signal sqlstate '45000' set message_text = "Centro non esistente";
	end if;	
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Film_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Film_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`Film`
FOR EACH ROW
BEGIN
	if
    (select count(*)
	from Settore as S
	where new.Settore = S.IDSettore and
		  new.Posizione = S.Posizione and
          new.Centro = S.IDCentro) = 0 then
			signal sqlstate '45000' set message_text = "Centro non esistente";
	end if;	
    
    if(select count(*) from Film as F 
	where new.Settore = F.Settore and
		  new.Posizione = F.Posizione and
          new.Centro = F.Centro) > 0 then
		signal sqlstate '45000' set message_text = "La posizione inserita è già occupata da un film";
	end if;
    
    if(new.CopieDisponibili < 0) then 
		signal sqlstate '45000' set message_text = "Non ci sono abbastanza copie";
    end if;
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Film_BEFORE_UPDATE` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Film_BEFORE_UPDATE`
BEFORE UPDATE ON `Noleggio_film`.`Film`
FOR EACH ROW
BEGIN
	if(new.CopieDisponibili < 0) then 
		signal sqlstate '45000' set message_text = "Non ci sono abbastanza copie";
    end if;
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Attore_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Attore_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`Attore`
FOR EACH ROW
BEGIN
	if
    (select count(*)
	from Film as F
	where new.Titolo = F.Titolo and
		  new.Regista = F.Regista) = 0 then
			signal sqlstate '45000' set message_text = "Film non esistente";
	end if;	
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Cliente_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Cliente_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`Cliente`
FOR EACH ROW
BEGIN

END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`FilmCorrispondeNoleggio_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`FilmCorrispondeNoleggio_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`FilmCorrispondeNoleggio`
FOR EACH ROW
BEGIN

END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`FilmRemake_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`FilmRemake_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`FilmRemake`
FOR EACH ROW
BEGIN
	if(select count(*) from Film where 
		Titolo = new.TitoloOriginale and
        Regista = new.RegistaOriginale) = 0 then 
		signal sqlstate '45000' set message_text = "Il film originale inserito non è registrato nel sistema";
    end if;
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Impiego_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Impiego_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`Impiego`
FOR EACH ROW
BEGIN
	if(new.CFImpiegato not in (select CF from Impiegato)) then
		signal sqlstate '45000' set message_text = "L'impiegato non è registrato nel sistema";
	end if;
    if(new.IDCentro not in (select IDCentro from Centro)) then
		signal sqlstate '45000' set message_text = "Il centro inserito non esiste";
	end if;
     if((new.CFImpiegato in (select CFImpiegato from Impiego)) and
	 (select count(*) from Impiego where 
     CFImpiegato = new.CFImpiegato 
     and new.DataInizio < DataFine 
     and new.DataInizio > DataInizio) > 0) then
		signal sqlstate '45000' set message_text = "L'impiegato ha già un turno in quel periodo";
     end if;
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Impiego_BEFORE_UPDATE` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Impiego_BEFORE_UPDATE`
BEFORE UPDATE ON `Noleggio_film`.`Impiego`
FOR EACH ROW
BEGIN
	if new.IDCentro not in (select IDCentro from Centro) then
		signal sqlstate '45000' set message_text = "Il centro inserito non esiste";
	end if;
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`PostaElettronica_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`PostaElettronica_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`PostaElettronica`
FOR EACH ROW
BEGIN
	if(new.Cliente not in (select CF from `Cliente`)) then 
		signal sqlstate '45000' set message_text = "L'utente selezionato non è nel sistema";
    end if;
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Telefono_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Telefono_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`Telefono`
FOR EACH ROW
BEGIN
	if(new.Cliente not in (select CF from `Cliente`)) then 
		signal sqlstate '45000' set message_text = "L'utente selezionato non è nel sistema";
    end if;
END$$


USE `Noleggio_film`$$
DROP TRIGGER IF EXISTS `Noleggio_film`.`Turno_BEFORE_INSERT` $$
USE `Noleggio_film`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `Noleggio_film`.`Turno_BEFORE_INSERT`
BEFORE INSERT ON `Noleggio_film`.`Turno`
FOR EACH ROW
BEGIN
	if(new.CFImpiegato not in (select CFImpiegato from Impiego)) then
		signal sqlstate '45000' set message_text = "L'impiegato non lavora presso alcun centro";
	end if;
    if(new.IDImpiego not in (select IDImpiego from Impiego)) then
		signal sqlstate '45000' set message_text = "L'impiego inserito non esiste e non possono essere assegnati dei turni";
	end if;
    
END$$


DELIMITER ;
