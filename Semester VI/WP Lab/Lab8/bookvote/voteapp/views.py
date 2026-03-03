from django.shortcuts import render
from django.views.decorators.csrf import csrf_protect

# In-memory vote storage
votes = {"Good": 0, "Satisfactory": 0, "Bad": 0}

@csrf_protect
def vote_page(request):
    total_votes = sum(votes.values())
    percentages = {}

    if request.method == "POST":
        choice = request.POST.get("choice")
        if choice in votes:
            votes[choice] += 1
        total_votes = sum(votes.values())

    if total_votes > 0:
        for key, value in votes.items():
            percentages[key] = round((value / total_votes) * 100, 2)

    return render(request, "vote.html", {
        "votes": votes,
        "percentages": percentages,
        "total": total_votes
    })