CREATE TABLE chat (
     chatID int PRIMARY KEY AUTO_INCREMENT,
     keyWords char(255)
     );

DELIMITER $$

CREATE PROCEDURE getWords ( IN id int, out words char(255) )
    BEGIN
     SELECT keyWords INTO words FROM chat
     WHERE chatID=id;
    END
$$

CREATE PROCEDURE addRow( IN id int, IN data char(255) )
    BEGIN
        INSERT INTO chat (chatID, keyWords) VALUES(NULL, data);
    END
$$

CREATE PROCEDURE deleteRow( IN id int )
    BEGIN
       DELETE from chat where chatID=id;
    END
$$

CREATE PROCEDURE updateRow( IN id INT, IN data CHAR(255) )
    BEGIN
        UPDATE chat
        SET keyWords = CONCAT(keyWords, ' ', data)
        WHERE chatID = id;
    END
$$

CREATE PROCEDURE get_data()
    BEGIN
        SELECT * FROM chat;
    END
$$

CREATE PROCEDURE get_size()
    BEGIN
        SELECT COUNT(*) FROM chat;
    END
$$