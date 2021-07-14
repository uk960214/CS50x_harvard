-- In 7.sql, write a SQL query to list all movies released in 2010 and their ratings, in descending order by rating. For movies with the same rating, order them alphabetically by title.


SELECT title, ratings.rating FROM movies INNER JOIN ratings ON ratings.movie_id=movies.id WHERE movies.year=2010 ORDER BY ratings.rating DESC, title;