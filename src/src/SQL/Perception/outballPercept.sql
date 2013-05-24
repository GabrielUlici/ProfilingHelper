-- This query shows the duration (intervat), in miliseconds, of the property out/ballPercept beeing locked and unlocked
-- To order uncomment the last line, change the word table with the table(s) name separated by comma and then choose ascending or descending

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp and EventSubject like 'perception%out/ballPercept')) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'perception%out/ballPercept'
--order by table asc|desc