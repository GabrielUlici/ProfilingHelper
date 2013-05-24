-- This query shows the duration (intervat), in miliseconds, of a property beeing locked and unlocked
-- To order uncomment the last line, change the word table with the table(s) name separated by comma and then choose ascending or descending
 
select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp and EventType<>'START' and EventType<>'END' and EventType not like 'RA%' )) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as Result
from profiling as tx
where EventType<>'START' and EventType<>'END' and EventType not like 'RA%'
--order by table asc|desc