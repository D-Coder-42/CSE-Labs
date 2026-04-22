from django.shortcuts import render, redirect, get_object_or_404
from .models import Human
from .forms import HumanForm

def index(request):
    humans = Human.objects.all()
    selected_human = None
    form = None

    if request.method == 'POST':
        if 'select' in request.POST:  # selecting from dropdown
            human_id = request.POST.get('human_id')
            selected_human = get_object_or_404(Human, id=human_id)
            form = HumanForm(instance=selected_human)

        elif 'update' in request.POST:  # update record
            human_id = request.POST.get('human_id')
            human = get_object_or_404(Human, id=human_id)
            form = HumanForm(request.POST, instance=human)
            if form.is_valid():
                form.save()
                return redirect('index')

        elif 'delete' in request.POST:  # delete record
            human_id = request.POST.get('human_id')
            human = get_object_or_404(Human, id=human_id)
            human.delete()
            return redirect('index')

    return render(request, 'index.html', {
        'humans': humans,
        'selected_human': selected_human,
        'form': form
    })
