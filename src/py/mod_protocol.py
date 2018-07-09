class Protocol: # I Want Enums
    class Replays:
        OK = "ok"
        BadOperation = "bo"
        Error = "er"
        NotAvailable = "na"
        NotInialized = "nint"
        Started = "strd"
        Stopped = "stpd"
        Busy = "busy"

    class Request:
        Initialize = "init"
        StartStream = "stst"
        StopStream = "spst"
        UpdateToken = "updt"
        State = "stat"
        UpdateService = "upds"
        UpdateStreamRes = "updr"
        CheckConnection = "ping"