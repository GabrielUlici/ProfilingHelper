-- This query shows the duration of a module
-- To see a specific module delete the comment in the where line and change the word "utils" but leave the % sign 
-- To order uncomment the last line, change the word table with the table(s) name separated by comma and then choose ascending or descending

select EventType, EventSubject, Other 
from profiling
where EventType='END' -- and EventSubject like '%utils%'
--order by table asc|desc