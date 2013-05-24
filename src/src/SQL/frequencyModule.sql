-- This query shows the scheduling frequency of a module
-- To order uncomment the last line, change the word table with the table(s) name separated by comma and then choose ascending or descending

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp and EventType<>'END' and EventType not like '%LOCK' and EventType not like 'RA%')) * (CASE EventType WHEN 'UNLOCK' THEN 0 WHEN 'LOCK' THEN 0 ELSE 1 END)  as Result
from profiling as tx
where EventType<>'END' and EventType not like '%LOCK' and EventType not like 'RA%'
--order by table asc|desc