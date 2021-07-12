-- In 6.sql, write a SQL query that lists the names of songs that are by Post Malone

SELECT songs.name FROM songs LEFT Join artists ON songs.artist_id=artists.id WHERE artists.name='Post Malone';