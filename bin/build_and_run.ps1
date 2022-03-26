cd ../build
cmake --build .
cd ../bin
$elapsed = [System.Diagnostics.Stopwatch]::StartNew() 
.\CodeCraft-2022
write-host "Total Elapsed Time: $($elapsed.Elapsed.ToString())" 