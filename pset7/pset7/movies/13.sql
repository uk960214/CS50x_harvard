-- In 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.

SELECT DISTINCT people.name FROM stars LEFT JOIN people ON stars.person_id=people.id
WHERE stars.movie_id IN (SELECT movie_id FROM stars JOIN people ON stars.person_id=people.id
WHERE people.name='Kevin Bacon' AND people.birth=1958) AND people.name != 'Kevin Bacon';