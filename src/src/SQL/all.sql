		-- Perception Module

-- This query shows the duration (intervat), in miliseconds, of the property out/ballPercept beeing locked and unlocked
 
select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'perception%out/ballPercept' 

-- This query shows the duration (intervat), in miliseconds, of the property out/ballInImage beeing locked and unlocked
 
select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'perception%out/ballInImage'  

-- This query shows the duration (intervat), in miliseconds, of the property out/goalOpponentLeftPost beeing locked and unlocked
 
select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'perception%out/goalOpponentLeftPost'

-- This query shows the duration (intervat), in miliseconds, of the property out/goalMineLeftPost beeing locked and unlocked

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'perception%out/goalMineLeftPost'

-- This query shows the duration (intervat), in miliseconds, of the property out/goalMineSeen beeing locked and unlocked

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'perception%out/goalMineSeen'

-- This query shows the duration (intervat), in miliseconds, of the property out/goalOpponentSeen beeing locked and unlocked

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'perception%out/goalOpponentSeen'

		-- Executor Module

-- This query shows the duration (intervat), in miliseconds, of the property in/coordinateBallField beeing locked and unlocked

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'executor%in/coordinateBallField'


-- This query shows the duration (intervat), in miliseconds, of the property in/coordinateBallImage beeing locked and unlocked

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'executor%in/coordinateBallImage'

-- This query shows the duration (intervat), in miliseconds, of the property in/goalMineLeftPost beeing locked and unlocked

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'executor%in/goalMineLeftPost'

-- This query shows the duration (intervat), in miliseconds, of the property in/goalOpponentLeftPost beeing locked and unlocked

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'executor%in/goalOpponentLeftPost'

-- This query shows the duration (intervat), in miliseconds, of the property in/goalMineSeen beeing locked and unlocked

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'executor%in/goalMineSeen'

-- This query shows the duration (intervat), in miliseconds, of the property in/goalOpponentSeen beeing locked and unlocked

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as "Result(milisec)"
from profiling as tx
where EventSubject like 'executor%in/goalOpponentSeen'


		-- Queries examples

-- This query shows the duration (intervat), in miliseconds, of a property beeing locked and unlocked
 
select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as Result
from profiling as tx
where EventType<>'START' and EventType<>'END' and EventType not like 'RA%' 


-- This query shows the required time of a property beeing read/written 

select ThreadTimestamp, EventType, EventSubject,
(ThreadTimestamp-(select max(ThreadTimestamp) from profiling as ty where ty.ThreadTimestamp<tx.ThreadTimestamp)) * (CASE EventType WHEN 'LOCK' THEN 0 ELSE 1 END)  as Result
from profiling as tx
where EventType<>'START' and EventType<>'END' and EventType not like 'RA%' 


-- This query shows the duration of a module

select EventType, EventSubject, Other 
from profiling
where EventType='END'


-- This query shows the scheduling frequency of a module

select Timestamp, EventType, EventSubject,
(Timestamp-(select max(Timestamp) from profiling as ty where ty.Timestamp<tx.Timestamp)) * (CASE EventType WHEN 'UNLOCK' THEN 0 WHEN 'LOCK' THEN 0 ELSE 1 END)  as Result
from profiling as tx
where EventType<>'END' and EventType not like '%LOCK' and EventType not like 'RA%'
