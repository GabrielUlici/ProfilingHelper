-- This query shows the duration (intervat), in miliseconds, of the property in/coordinateBallImage beeing locked and unlocked
-- To order uncomment the last line, change the word table with the table(s) name separated by comma and then choose ascending or descending

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp and EventSubject like 'executor%in/coordinateBallImage')) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'executor%in/coordinateBallImage'
--order by table asc|desc