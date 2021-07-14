-- In 12.sql, write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.

SELECT title FROM movies LEFT JOIN(stars LEFT JOIN people ON stars.person_id=people.id) ON stars.movie_id=movies.id WHERE people.name='Helena Bonham Carter' OR people.name='Johnny Depp' GROUP BY title HAVING COUNT(stars.person_id) > 1;