-- In 10.sql, write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.

SELECT name FROM people
WHERE id IN
(SELECT directors.person_id FROM movies
JOIN directors ON directors.movie_id=movies.id
JOIN ratings ON ratings.movie_id=movies.id
WHERE ratings.rating >= 9.0);