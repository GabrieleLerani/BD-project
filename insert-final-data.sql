use Noleggio_film;


call insert_user('ABCDEF4439','Giacomo','gianno','2001-12-05','Roma','giacomo','giacomo');
call insert_user('BBCDEF4439','Luca','loti','2001-06-05','Roma','luca','luca');
call insert_user('CBCDEF4439','Antonio','anto','2000-03-09','Roma','antonio','antonio');
call insert_user('DBCDEF4439','Amalia','lobianco','2002-12-12','Palermo','amalia','amalia');
call insert_user('EBCDEF4439','Fabio','mani','2000-05-07','Genzano','fabio','fabio');
call insert_user('FBCDEF4439','Marco','acro','1996-02-24','Roma','marco','marco');



INSERT INTO Utente(Username,password,Ruolo) VALUES('Gabriele','gabriele','manager');
insert into Impiegato values('LRNGRL1234','Gabriele','diploma','069320845','9');
insert into Manager values('Gabriele','LRNGRL1234');
insert into Centro values('1','Gabriele','LRNGRL1234','3423758183','g.ler@gmail.com','Mario mo','1','00041');
-- Inserisci settore
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '1');
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '2');
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '3');
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '4');
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '5');
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '6');
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '7');
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '8');
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '9');
INSERT INTO `Noleggio_film`.`Settore` (`IDSettore`, `IDCentro`, `Posizione`) VALUES ('1', '1', '10');

call insert_film('Inception','Nolan','20','2010-09-13','100','classico','1','1','1');
call insert_film('Forrest gump','Zemerckis','15','1984-09-13','150','classico','1','1','2');
