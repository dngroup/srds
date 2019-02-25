var appRouter = function(app) {

	app.get("/example", function(req, res) {
		var example = {
		    "toto": "titi",
		    "tata": "tutu",
		}
		return res.send(example);
		}
	});

	app.post("/example", function(req, res) {
		if(!req.body.toto || !req.body.tata) {
		    return res.send({"status": "error", "message": "missing a parameter"});
		} else {
		    return res.send(req.body);
		}
	});

}

module.exports = appRouter;

