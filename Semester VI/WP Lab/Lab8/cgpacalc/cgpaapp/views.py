from django.shortcuts import render, redirect
from django.views.decorators.csrf import csrf_protect

@csrf_protect
def page1(request):
    if request.method == "POST":
        name = request.POST.get("name")
        total_marks = request.POST.get("total_marks")

        if name and total_marks:
            try:
                total_marks = float(total_marks)
                cgpa = total_marks / 50
            except ValueError:
                cgpa = None

            # Store in session
            request.session['name'] = name
            request.session['cgpa'] = cgpa

            return redirect('page2')

    return render(request, 'page1.html')


def page2(request):
    name = request.session.get('name')
    cgpa = request.session.get('cgpa')

    return render(request, 'page2.html', {
        'name': name,
        'cgpa': cgpa
    })
