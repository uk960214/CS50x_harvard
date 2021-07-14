-- In 9.sql, write a SQL query to list the names of all people who starred in a movie released in 2004, ordered by birth year.

SELECT name FROM people
WHERE id IN (
SELECT DISTINCT stars.person_id
FROM movies JOIN stars
ON stars.movie_id=movies.id
WHERE movies.year=2004)
ORDER BY birth;