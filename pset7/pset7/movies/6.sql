-- In 6.sql, write a SQL query to determine the average rating of all movies released in 2012.

SELECT AVG(ratings.rating) FROM ratings LEFT JOIN movies ON ratings.movie_id=movies.id WHERE movies.year=2012;