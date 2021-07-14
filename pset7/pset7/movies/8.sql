-- In 8.sql, write a SQL query to list the names of all people who starred in Toy Story.

SELECT people.name FROM movies INNER JOIN (stars INNER JOIN people ON stars.person_id=people.id) ON stars.movie_id=movies.id WHERE title='Toy Story';