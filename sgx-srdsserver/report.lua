done = function(summary, latency, requests)
  f = io.open("logs/wrk.csv", "a+")
--  f:write("time_started,min_requests,max_requests,mean_requests,count_requests,min_latency,max_latency,mean_latency,stdev,50th,90th,99th,99.999th,duration,requests,bytes,connect_errors,read_errors,write_errors,status_errors,timeouts\n")
  f:write(string.format("%s,%f,%f,%f,%s,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%d\n",
    os.date("!%Y-%m-%dT%TZ"),
    requests.min,   -- minimum latency
    requests.max,   -- max latency
    requests.mean,  -- mean of latency
    requests, -- raw value and count
    latency.min,    -- minimum latency
    latency.max,    -- max latency
    latency.mean,   -- mean of latency
    latency.stdev,  -- standard deviation of latency

    latency:percentile(50),     -- 50percentile latency
    latency:percentile(90),     -- 90percentile latency
    latency:percentile(99),     -- 99percentile latency
    latency:percentile(99.999), -- 99.999percentile latency

    summary["duration"],          -- duration of the benchmark
    summary["requests"],          -- total requests during the benchmark
    summary["bytes"],             -- total received bytes during the benchmark

    summary["errors"]["connect"], -- total socket connection errors
    summary["errors"]["read"],    -- total socket read errors
    summary["errors"]["write"],   -- total socket write errors
    summary["errors"]["status"],  -- total socket write errors
    summary["errors"]["timeout"]  -- total request timeouts
    ))

  f:close()
end

