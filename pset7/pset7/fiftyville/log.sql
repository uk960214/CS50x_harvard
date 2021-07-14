-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Tables
-- airports crime_scene_reports people
-- atm_transactions flights phone_calls
-- bank_accounts interviews
-- courthouse_security_logs passengers

-- Get crime description from the crime scene report
SELECT description FROM crime_scene_reports WHERE year=2020 AND month=7 AND day=28 AND street='Chamberlin Street';

-- Result: Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

-- Check security log of courthouse
SELECT minute, license_plate FROM courthouse_security_logs WHERE year=2020 AND month=7 AND day=28 AND hour=10 AND activity='exit' AND minute < 25 AND minute > 15;

-- Result:
--minute | license_plate
--16 | 5P2BI95
--18 | 94KL13X
--18 | 6P58WS2
--19 | 4328GD8
--20 | G412CB7
--21 | L93JTIZ
--23 | 322W7JE
--23 | 0NTHK55

-- Get interview transcripts
SELECT name, transcript FROM interviews WHERE year=2020 AND month=7 AND day=28;

-- Result:
-- Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away.
-- Eugene | Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute.
--           In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--           The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Get names using account numbers from ATM on Fifer street
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN (
SELECT account_number FROM atm_transactions
WHERE year=2020 AND month=7 AND day=28
AND transaction_type='withdraw'
AND atm_location='Fifer Street');

-- Result:
-- Ernest
-- Russell
-- Roy
-- Bobby
-- Elizabeth
-- Danielle
-- Madison
-- Victoria

-- Get call logs
SELECT caller, receiver FROM phone_calls
WHERE year=2020 AND month=7 AND day=28
AND duration<60;

-- Get match using account holders, license plate, and phone number
SELECT name FROM people
WHERE name IN (
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN (
SELECT account_number FROM atm_transactions
WHERE year=2020 AND month=7 AND day=28
AND transaction_type='withdraw'
AND atm_location='Fifer Street'))
AND license_plate IN (
SELECT license_plate FROM courthouse_security_logs
WHERE year=2020 AND month=7 AND day=28 AND hour=10
AND activity='exit' AND minute < 25 AND minute > 15)
AND phone_number IN (
SELECT caller FROM phone_calls
WHERE year=2020 AND month=7 AND day=28
AND duration<60);

-- Result:
--  Russel
--  Ernest


-- Get receiver of phone calls from two suspects
SELECT name FROM phone_calls
JOIN people ON phone_calls.receiver=people.phone_number
WHERE year=2020 AND month=7 AND day=28
AND duration<60
AND caller=(
SELECT phone_number FROM people
WHERE name='Russell');

-- Result: Philip

SELECT name FROM phone_calls
JOIN people ON phone_calls.receiver=people.phone_number
WHERE year=2020 AND month=7 AND day=28
AND duration<60
AND caller=(
SELECT phone_number FROM people
WHERE name='Ernest');

-- Result: Berthold


-- Find earliest flight tommorow and search passenger list and find destination

SELECT origin.city , destin.city , hour, minute FROM flights
JOIN airports origin ON origin_airport_id = origin.id
JOIN airports destin ON destination_airport_id = destin.id
WHERE year=2020 AND month=7 AND day=29
ORDER BY hour, minute LIMIT 1;

-- Result: Fiftyville | London | 8 | 20


SELECT name FROM people
WHERE passport_number IN (
SELECT passport_number FROM passengers
WHERE flight_id = (
SELECT id FROM flights
WHERE year=2020 AND month=7 AND day=29
ORDER BY hour, minute LIMIT 1));

-- Result
--  Bobby
--  Roger
--  Madison
--  Danielle
--  Evelyn
--  Edward
--  Ernest
--  Doris

-- Final cross check
SELECT name FROM people
WHERE name IN (
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN (
SELECT account_number FROM atm_transactions
WHERE year=2020 AND month=7 AND day=28
AND transaction_type='withdraw'
AND atm_location='Fifer Street'))
AND license_plate IN (
SELECT license_plate FROM courthouse_security_logs
WHERE year=2020 AND month=7 AND day=28 AND hour=10
AND activity='exit' AND minute < 25 AND minute > 15)
AND phone_number IN (
SELECT caller FROM phone_calls
WHERE year=2020 AND month=7 AND day=28
AND duration<60)
AND passport_number IN (
SELECT passport_number FROM passengers
WHERE flight_id = (
SELECT id FROM flights
WHERE year=2020 AND month=7 AND day=29
ORDER BY hour, minute LIMIT 1));

-- Result: Ernest