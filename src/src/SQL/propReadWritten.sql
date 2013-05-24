-- This query shows the required time of a property beeing read/written 
-- To order uncomment the last line, change the word table with the table(s) name separated by comma and then choose ascending or descending

select ThreadTimestamp, EventType, EventSubject,
(ThreadTimestamp-(select ThreadTimestamp from profiling as ty where ty.ThreadTimestamp=tx.ThreadTimestamp and EventType<>'START' and EventType<>'END' and EventType not like 'RA%')) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as Result
from profiling as tx
where EventType<>'START' and EventType<>'END' and EventType not like 'RA%' 
--order by table asc|desc