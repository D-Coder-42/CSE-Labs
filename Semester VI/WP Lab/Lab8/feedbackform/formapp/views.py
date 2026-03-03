from django.shortcuts import render
from .forms import FeedbackForm

def feedback_view(request):
    message = ""
    if request.method == "POST":
        form = FeedbackForm(request.POST)
        if form.is_valid():
            name = form.cleaned_data['student_name']
            sex = form.cleaned_data['sex']

            prefix = "Mr." if sex == "Male" else "Miss."

            message = f"Thanks {prefix} {name} for your feedback."
    else:
        form = FeedbackForm()
    return render(request, "feedback.html", {"form": form, "message": message})
